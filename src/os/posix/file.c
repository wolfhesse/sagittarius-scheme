/* file.c                                          -*- mode:c; coding:utf-8; -*-
 *
 *   Copyright (c) 2010-2015  Takashi Kato <ktakashi@ymail.com>
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions
 *   are met:
 *
 *   1. Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 *   TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 *   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 *   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  $Id: $
 */
#include <string.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <dirent.h>
#include <limits.h>
#include "sagittarius/config.h"
#ifdef HAVE_SYS_MMAN_H
# include <sys/mman.h>
#endif
#define LIBSAGITTARIUS_BODY
#include "sagittarius/file.h"
#include "sagittarius/unicode.h"
#include "sagittarius/pair.h"
#include "sagittarius/string.h"
#include "sagittarius/error.h"
#include "sagittarius/symbol.h"
#include "sagittarius/number.h"
#include "sagittarius/system.h"

enum {
  INVALID_HANDLE_VALUE = -1,
};

typedef struct FDRec
{
  int fd;
  int errorno;
} FD;

#define SG_FILE_DEP(f) (SG_FILE(f)->osdependance)
#define SG_FD(o)  ((FD*)(SG_FILE_DEP(o)))
#define setLastError(file) (SG_FD(file)->errorno = errno)

static SgObject get_last_error_message(SgObject file)
{
  const char *msg = strerror(SG_FD(file)->errorno);
  return Sg_MakeStringC(msg);  
}

static int posix_open(SgObject self, SgString *path, int flags)
{
  int mode = 0;
  SG_FILE(self)->name = path->value;
  if ((flags & SG_READ) && (flags & SG_WRITE)) {
    mode |= O_RDWR;
  } else {
    if (flags & SG_WRITE) {
      mode |= O_WRONLY;
    }
    if (flags & SG_READ) {
      mode |= O_RDONLY;
    }
  }
  if (flags & SG_CREATE) {
    mode |= O_CREAT;
  }
  if (flags & SG_TRUNCATE) {
    mode |= O_TRUNC;
  }
  SG_FD(self)->fd = open(Sg_Utf32sToUtf8s(path), mode, 0644);
  setLastError(self);
  return SG_FILE_VTABLE(self)->isOpen(self);
}

static int posix_is_open(SgObject self)
{
  return SG_FD(self)->fd != INVALID_HANDLE_VALUE;
}

static int posix_close(SgObject self)
{
  if (SG_FD(self)->fd == 0 ||
      SG_FD(self)->fd == 1 ||
      SG_FD(self)->fd == 2) {
    /* we never close standard fd */
    return TRUE;
  }
  if (SG_FILE_VTABLE(self)->isOpen(self)) {
    const int isOK = close(SG_FD(self)->fd) != 0;
    setLastError(self);
    SG_FD(self)->fd = INVALID_HANDLE_VALUE;
    return isOK;
  }
  return FALSE;
}

static int posix_can_close(SgObject self)
{
  return !(SG_FD(self)->fd == 0 ||
	   SG_FD(self)->fd == 1 ||
	   SG_FD(self)->fd == 2);
}


static int64_t posix_seek(SgObject self, int64_t offset, Whence whence)
{
  int w = SEEK_SET;
  int64_t ret;
  switch (whence) {
  case SG_BEGIN:
    w = SEEK_SET;
    break;
  case SG_CURRENT:
    w = SEEK_CUR;
    break;
  case SG_END:
    w = SEEK_END;
    break;
  }
  ret = lseek(SG_FD(self)->fd, offset, w);
  setLastError(self);
  return ret;
}

static int64_t posix_tell(SgObject self)
{
  return posix_seek(self, 0, SG_CURRENT);
}

static int64_t posix_read(SgObject self, uint8_t *buf, int64_t size)
{
  int64_t result = 0;
  ASSERT(posix_is_open(self));
  errno = 0;
  do {
    result = read(SG_FD(self)->fd, buf, size);
  } while (result < 0 && errno == EINTR);
  setLastError(self);
  if (result < 0) {
    /* TODO this must be &io/error */
    SgObject err = get_last_error_message(self);
    Sg_IOReadError(SG_INTERN("file reader"), err, SG_FALSE, self);
  }
  return result;
}

static int64_t posix_write(SgObject self, uint8_t *buf, int64_t size)
{
  int64_t result;
  ASSERT(posix_is_open(self));
  do {
    result = write(SG_FD(self)->fd, buf, size);
  } while (result < 0 && errno == EINTR);
  setLastError(self);
  if (result < 0) {
    SgObject err = get_last_error_message(self);
    Sg_IOWriteError(SG_INTERN("file writer"), err, SG_FALSE, self);
  }
  return result;
}

static int64_t posix_size(SgObject self)
{
  struct stat st;
  const int result = fstat(SG_FD(self)->fd, &st);
  setLastError(self);
  if (result != 0) {
    return -1;
  } else {
    return st.st_size;
  }
}

static int posix_ready(SgObject self)
{
#ifdef HAVE_SELECT
  struct timeval tm = {0 , 0};
  fd_set fds;
  int state;
  FD_ZERO(&fds);
  /* what would be the best behaviour if the given FD is larger than
     FD_SETSIZE? I don't like predicates raise an error, so for now
     I only return #f. */
  if (SG_FD(self)->fd >= FD_SETSIZE) return FALSE;

  FD_SET(SG_FD(self)->fd, &fds);
  state = select(SG_FD(self)->fd + 1, &fds, NULL, NULL, &tm);
  if (state < 0) {
    /* in this case it's basically bad file descriptor means
       either it's closed or exceed the FD_SETSIZE. 
       However, I don't think predicate should raise an error
       so just return FALSE. */
#if 0
    SgObject err;
    if (errno == EINTR) return FALSE;
    setLastError(self);
    err = Sg_Sprintf(UC("%A [FD %d]"), get_last_error_message(self),
		     SG_FD(self)->fd);
    Sg_IOError(-1, SG_INTERN("file ready"), err, self, self);
#endif
    return FALSE;
  }
  return (state != 0);
#else
  /* default true */
  return TRUE;
#endif  
}

static SgFileTable vtable = {
  posix_read,
  posix_write,
  posix_seek,
  posix_tell,
  posix_size,
  posix_is_open,
  posix_open,
  posix_close,
  posix_can_close,
  posix_ready
};

static SgFile* init_file(SgFile *file, int handle)
{
  FD     *fd = SG_NEW_ATOMIC(FD);
  SG_SET_CLASS(file, SG_CLASS_FILE);
  fd->fd = handle;
  fd->errorno = 0;
  file->osdependance = (void*)fd;
  return file;
}

SgObject Sg_MakeFile()
{
  SgFile *z = SG_NEW(SgFile);
  return Sg_InitFile(z);
}

SgObject Sg_InitFile(SgFile *file)
{
  init_file(file, INVALID_HANDLE_VALUE);
  SG_FILE_VTABLE(file) = &vtable;
  return SG_OBJ(file);
}

SgObject Sg_FileErrorMessage(SgObject file)
{
  return get_last_error_message(file);
}

int Sg_LockFile(SgObject file, enum SgFileLockType mode)
{
  struct flock fl;
  int cmd = F_SETLKW;
  /* we lock the whole file */
  fl.l_whence = SEEK_SET;
  fl.l_start = 0;
  fl.l_len = 0;
  fl.l_type = F_RDLCK;		/* default */
  if (mode & SG_EXCLUSIVE) fl.l_type = F_WRLCK;
  if (mode & SG_DONT_WAIT) cmd = F_SETLK;
  if (fcntl(SG_FD(file)->fd, cmd, &fl)) {
    setLastError(file);
    return FALSE;
  }
  return TRUE;
}

int Sg_UnlockFile(SgObject file)
{
  struct flock fl;
  /* we lock the whole file */
  fl.l_whence = SEEK_SET;
  fl.l_start = 0;
  fl.l_len = 0;
  fl.l_type = F_UNLCK;
  if (fcntl(SG_FD(file)->fd, F_SETLK, &fl)) {
    setLastError(file);
    return FALSE;
  }
  return TRUE;
}

static SgFile *stdOut = NULL;
static SgFile *stdIn = NULL;
static SgFile *stdError = NULL;

SgObject Sg_StandardOut()
{
  if (!stdOut) {
    stdOut = Sg_MakeFileFromFD(1);
    stdOut->name = UC("stdout");
  }
  return SG_OBJ(stdOut);
}

SgObject Sg_StandardIn()
{
  if (!stdIn) {
    stdIn = Sg_MakeFileFromFD(0);
    stdIn->name = UC("stdin");
  }
  return SG_OBJ(stdIn);
}

SgObject Sg_StandardError()
{
  if (!stdError) {
    stdError = Sg_MakeFileFromFD(2);
    stdError->name = UC("stderr");
  }
  return SG_OBJ(stdError);
}

SgObject Sg_MakeFileFromFD(uintptr_t handle)
{
  SgFile *f = SG_NEW(SgFile);
  init_file(f, (int)handle);
  f->name = UC("fd");
  SG_FILE_VTABLE(f) = &vtable;
  return SG_OBJ(f);
}

int Sg_IsUTF16Console(SgObject file)
{
  return FALSE;
}

int Sg_FileExistP(SgString *path)
{
  char *utf8path = Sg_Utf32sToUtf8s(path);
  return access(utf8path, F_OK) == 0; 
}

int Sg_DeleteFile(SgString *path)
{
  return remove(Sg_Utf32sToUtf8s(path));
}

int Sg_CopyFile(SgString *src, SgString *dst, int overwriteP)
{
#define check_file_exists					\
  do {								\
    if (Sg_FileExistP(dst) && !overwriteP) return FALSE;	\
  } while (0)							\

  int fps, fpd, r, e;
  struct stat st;
  const char *source = Sg_Utf32sToUtf8s(src), *dest = Sg_Utf32sToUtf8s(dst);
#ifdef HAVE_MMAP
  void *bufs, *bufd;

  check_file_exists;
  if((fps = open(source, O_RDONLY)) == -1) {
    Sg_IOError(-1, SG_INTERN("copy-file"),
	       SG_MAKE_STRING("failed to open src file"),
	       SG_FALSE, dst);
    return FALSE;		/* dummy */
  }
  if((fpd = open(dest, O_RDWR | O_CREAT | O_TRUNC, 0644)) == -1) {
    close(fps);
    Sg_IOError(-1, SG_INTERN("copy-file"),
	       SG_MAKE_STRING("failed to open dst file"),
	       SG_FALSE, dst);
    return FALSE;		/* dummy */
  }
  if(fstat(fps, &st) == -1) {
    e = errno;
    close(fpd);
    close(fps);
    Sg_SystemError(e, UC("failed to fstat"));
    return FALSE;
  }
  if(pwrite(fpd, "", 1, st.st_size - 1) != 1) {
    close(fpd);
    close(fps);
    Sg_IOError(-1, SG_INTERN("copy-file"),
	       SG_MAKE_STRING("failed to create dst buffer"),
	       SG_FALSE, dst);
    return FALSE;		/* dummy */
  }
  if((bufs=mmap(0, st.st_size, PROT_READ, MAP_SHARED, fps, 0)) == MAP_FAILED) {
    e = errno;
    close(fpd);
    close(fps);
    Sg_SystemError(e, UC("failed to mmap (src)"));
    return FALSE;		/* dummy */
  }
  if((bufd=mmap(0, st.st_size, PROT_WRITE, MAP_SHARED, fpd, 0)) == MAP_FAILED) {
    e = errno;
    close(fpd);
    close(fps);
    Sg_SystemError(e, UC("failed to mmap (dst)"));
    return FALSE;		/* dummy */
  }

  memcpy(bufd, bufs, st.st_size);

  munmap(bufd, st.st_size);
  munmap(bufs, st.st_size);

#else
#define BSIZE 8000
  ssize_t bytes;
  char buffer[BSIZE];

  check_file_exists;
  if((fps = open(source, O_RDONLY)) == -1) {
    Sg_IOError(-1, SG_INTERN("copy-file"),
	       SG_MAKE_STRING("failed to open src file"),
	       SG_FALSE, dst);
    return FALSE;		/* dummy */
  }
  if((fpd = open(dest, O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1) {
    close(fps);
    Sg_IOError(-1, SG_INTERN("copy-file"),
	       SG_MAKE_STRING("failed to open dst file"),
	       SG_FALSE, dst);
    return FALSE;		/* dummy */
  }
  if(fstat(fps, &st) == -1) {
    e = errno;
    close(fpd);
    close(fps);
    Sg_SystemError(e, UC("failed to fstat"));
    return FALSE;
  }

  while((bytes = read(fps, buffer, BSIZE)) > 0)
    write(fpd, buffer, bytes);

#undef BSIZE
#endif
  Sg_ChangeFileMode(dst, st.st_mode);
  r = chown(dest, st.st_uid, st.st_gid);
  e = errno;
  close(fpd);
  close(fps);

  if (r < 0) {
    Sg_SystemError(e, UC("chown: %A"), Sg_GetLastErrorMessageWithErrorCode(e));
    return FALSE;
  }
  return TRUE;
}

/* Originally from Mosh start */
int Sg_FileWritableP(SgString *path)
{
  return access(Sg_Utf32sToUtf8s(path), W_OK | F_OK) == 0;
}

int Sg_FileReadableP(SgString *path)
{
  return access(Sg_Utf32sToUtf8s(path), R_OK) == 0;
}

int Sg_FileRegularP(SgString *path)
{
  struct stat st;
  if (stat(Sg_Utf32sToUtf8s(path), &st) == 0) {
    return S_ISREG(st.st_mode);
  }
  return FALSE;
}

int Sg_FileSymbolicLinkP(SgString *path)
{
  struct stat st;
  if (lstat(Sg_Utf32sToUtf8s(path), &st) == 0) {
    return S_ISLNK(st.st_mode);
  }
  return FALSE;
}

int Sg_FileExecutableP(SgString *path)
{
  return access(Sg_Utf32sToUtf8s(path), X_OK) == 0;
}

int Sg_DirectoryP(SgString *path)
{
  struct stat st;
  if (stat(Sg_Utf32sToUtf8s(path), &st) == 0) {
    return S_ISDIR(st.st_mode) ? TRUE : FALSE;
  }
  return FALSE;
}

int Sg_DeleteFileOrDirectory(SgString *path)
{
  return remove(Sg_Utf32sToUtf8s(path)) == 0;
}

int Sg_FileRename(SgString *oldpath, SgString *newpath)
{
  return rename(Sg_Utf32sToUtf8s(oldpath), Sg_Utf32sToUtf8s(newpath)) == 0;
}

void Sg_ChangeFileMode(SgString *path, int mode)
{
  if (chmod(Sg_Utf32sToUtf8s(path), mode) < 0) {
    Sg_IOError(SG_IO_FILE_NOT_EXIST_ERROR, SG_INTERN("change-file-mode"),
	       Sg_GetLastErrorMessage(), path, SG_FALSE);
  }
}

int Sg_CreateSymbolicLink(SgString *oldpath, SgString *newpath)
{
  return symlink(Sg_Utf32sToUtf8s(oldpath), Sg_Utf32sToUtf8s(newpath)) == 0;
}

int Sg_CreateDirectory(SgString *path)
{
  return mkdir(Sg_Utf32sToUtf8s(path), S_IRWXU | S_IRWXG | S_IRWXO) == 0;
}

SgObject Sg_FileModifyTime(SgString *path)
{
    struct stat st;
    if (stat(Sg_Utf32sToUtf8s(path), &st) == 0) {
#if __DARWIN_64_BIT_INO_T
        return Sg_Add(Sg_MakeIntegerFromS64(st.st_mtimespec.tv_nsec),
		      Sg_Mul(Sg_MakeIntegerFromS64(1000000000),
			     Sg_MakeIntegerFromS64(st.st_mtimespec.tv_sec)));
#elif defined(_BSD_SOURCE) || defined(_SVID_SOURCE)
        return Sg_Add(Sg_MakeInteger(st.st_mtim.tv_nsec),
		      Sg_Mul(Sg_MakeIntegerFromS64(1000000000),
			     Sg_MakeIntegerFromS64(st.st_mtim.tv_sec)));
#else
        return Sg_Mul(Sg_MakeIntegerFromS64(1000000000),
		      Sg_MakeIntegerFromS64(st.st_mtime));
#endif
    }
    return SG_UNDEF;
}

SgObject Sg_FileAccessTime(SgString *path)
{
    struct stat st;
    if (stat(Sg_Utf32sToUtf8s(path), &st) == 0) {
#if __DARWIN_64_BIT_INO_T
        return Sg_Add(Sg_MakeIntegerFromS64(st.st_atimespec.tv_nsec),
		      Sg_Mul(Sg_MakeIntegerFromS64(1000000000),
			     Sg_MakeIntegerFromS64(st.st_atimespec.tv_sec)));
#elif defined(_BSD_SOURCE) || defined(_SVID_SOURCE)
        return Sg_Add(Sg_MakeInteger(st.st_atim.tv_nsec),
		      Sg_Mul(Sg_MakeIntegerFromS64(1000000000),
			     Sg_MakeIntegerFromS64(st.st_atim.tv_sec)));
#else
        return Sg_Mul(Sg_MakeIntegerFromS64(1000000000),
		      Sg_MakeIntegerFromS64(st.st_atime));
#endif
    }
    return SG_UNDEF;
}

SgObject Sg_FileChangeTime(SgString *path)
{
    struct stat st;
    if (stat(Sg_Utf32sToUtf8s(path), &st) == 0) {
#if __DARWIN_64_BIT_INO_T
        return Sg_Add(Sg_MakeIntegerFromS64(st.st_ctimespec.tv_nsec),
		      Sg_Mul(Sg_MakeIntegerFromS64(1000000000),
			     Sg_MakeIntegerFromS64(st.st_ctimespec.tv_sec)));
#elif defined(_BSD_SOURCE) || defined(_SVID_SOURCE)
        return Sg_Add(Sg_MakeInteger(st.st_ctim.tv_nsec),
		      Sg_Mul(Sg_MakeIntegerFromS64(1000000000),
			     Sg_MakeIntegerFromS64(st.st_ctim.tv_sec)));
#else
        return Sg_Mul(Sg_MakeIntegerFromS64(1000000000),
		      Sg_MakeIntegerFromS64(st.st_ctime));
#endif
    }
    return SG_UNDEF;
}

SgObject Sg_FileSize(SgString *path)
{
  struct stat st;
  if (stat(Sg_Utf32sToUtf8s(path), &st) == 0) {
    return Sg_MakeIntegerFromS64(st.st_size);
  }
  return SG_UNDEF;
}

SgObject Sg_ReadDirectory(SgString *path)
{
  DIR* dir;
  struct dirent* entry;
  SgObject h = SG_NIL, t = SG_NIL;
  if (NULL == (dir = opendir(Sg_Utf32sToUtf8s(path)))) {
    return SG_FALSE;
  }
  for (entry = readdir(dir); entry != NULL; entry = readdir(dir)) {
    SG_APPEND1(h, t, Sg_MakeStringC(entry->d_name));
  }
  closedir(dir);
  return h;
}

SgObject Sg_CurrentDirectory()
{
  char buf[MAXPATHLEN];
  if (getcwd(buf, MAXPATHLEN) == NULL) {
    Sg_IOError(-1, SG_INTERN("current-directory"),
	       Sg_GetLastErrorMessage(), SG_FALSE, SG_FALSE);
    return SG_UNDEF;
  }
  return Sg_Utf8sToUtf32s(buf, strlen(buf));
}

void Sg_SetCurrentDirectory(SgString *path)
{
  if (chdir(Sg_Utf32sToUtf8s(path)) < 0) {
    Sg_IOError(-1, SG_INTERN("set-current-directory"),
	       Sg_GetLastErrorMessage(), SG_FALSE, SG_FALSE);
  }
}

SgObject Sg_DirectoryName(SgString *path)
{
  int size = SG_STRING_SIZE(path), i;
  for (i = size-1; i >= 0; i--) {
    if (SG_STRING_VALUE_AT(path, i) == '/') break;
  }
  if (i <= 0) return SG_FALSE;
  return Sg_Substring(path, 0, i);
}

SgObject Sg_BuildPath(SgString *path, SgString *file)
{
  int psize = SG_STRING_SIZE(path), fsize = SG_STRING_SIZE(file);
  int i, j, offset = 1;
  SgObject ret;
  if (SG_STRING_VALUE_AT(path, psize-1) == '/') offset--;
  ret = Sg_ReserveString(psize + fsize + offset, 0);
  for (i = 0; i < psize; i++) {
    SG_STRING_VALUE_AT(ret, i) = SG_STRING_VALUE_AT(path, i);
  }
  if (offset) {
    SG_STRING_VALUE_AT(ret, i++) = '/';
  }
  for (j = 0; j < fsize; i++, j++) {
    SG_STRING_VALUE_AT(ret, i) = SG_STRING_VALUE_AT(file, j);
  }
  return ret;
}

int Sg_AbsolutePathP(SgString *path)
{
  return (SG_STRING_VALUE_AT(path, 0) == '/');
}

SgObject Sg_AbsolutePath(SgString *path)
{
  char buf[PATH_MAX];
  char *ret = realpath(Sg_Utf32sToUtf8s(path), buf);
  if (ret) {
    return Sg_MakeStringC(buf);
  }
  return SG_FALSE;
}

SgObject Sg_InstalledDirectory()
{
  /* On POSIX environment, I don't think there are somebody interested in this
     path. So just return #f */
  return SG_FALSE;
}

int Sg_CopyAccessControl(SgString *src, SgString *dst)
{
  struct stat st;
  const char *source = Sg_Utf32sToUtf8s(src), *dest = Sg_Utf32sToUtf8s(dst);
  if (stat(source, &st) == 0) {
    int r;
    /* TODO check the return value... */
    (void)chmod(dest, st.st_mode);
    r = chown(dest, st.st_uid, st.st_gid);
    if (r < 0) {
      Sg_SystemError(errno, UC("copy-access-control: %A"),
		     Sg_GetLastErrorMessageWithErrorCode(errno));
    }
    return TRUE;
  }
  /* TODO should this be error? */
  return FALSE;
}


/*
  end of file
  Local Variables:
  coding: utf-8-unix
  End:
*/
