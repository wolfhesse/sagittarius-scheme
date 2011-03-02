/* -*- C -*- */
/*
 * subr.h
 *
 *   Copyright (c) 2010  Takashi Kato <ktakashi@ymail.com>
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
#ifndef SAGITTARIUS_SUBR_H_
#define SAGITTARIUS_SUBR_H_

#include "sagittariusdefs.h"

typedef SgObject SgSubrProc(SgObject *args, int argc, void *user_data);

/* TODO think about it...*/
struct SgProcedureRec
{
  SG_HEADER;
  unsigned int required : 16;
  unsigned int optional : 8;
  unsigned int type     : 2;	/* SgProcedureType */
  SgObject     name;
  SgObject     inliner;		/* #f, or instruction */
};

enum SgProcedureType {
  SG_PROC_SUBR,
  SG_PROC_CLOSURE
};

#define SG_PROCEDURE(obj)          ((SgProcedure*)(obj))
#define SG_PROCEDUREP(obj)         (SG_PTRP(obj) && IS_TYPE(obj, TC_PROCEDURE))
#define SG_PROCEDURE_REQUIRED(obj) SG_PROCEDURE(obj)->required
#define SG_PROCEDURE_OPTIONAL(obj) SG_PROCEDURE(obj)->optional
#define SG_PROCEDURE_TYPE(obj)     SG_PROCEDURE(obj)->type
#define SG_PROCEDURE_NAME(obj)     SG_PROCEDURE(obj)->name
#define SG_PROCEDURE_INLINER(obj)  SG_PROCEDURE(obj)->inliner

#define SG_PROCEDURE_INIT(obj, req, opt, typ, name)	\
  SG_PROCEDURE_REQUIRED(obj) = (req),			\
  SG_PROCEDURE_OPTIONAL(obj) = (opt),			\
  SG_PROCEDURE_TYPE(obj) = (typ),			\
  SG_PROCEDURE_NAME(obj) = (name),			\
  SG_PROCEDURE_INLINER(obj) = SG_FALSE			\

#define SG__PROCEDURE_INITIALIZER(hdr, req, opt, type, name, inliner)	\
  { (hdr), (req), (opt), (type), (name), (inliner) }

/* This is just container for procedure */
struct SgSubrRec
{
  SgProcedure  common;
  SgSubrProc  *func;
  void        *data;
  SgWord       returnCode[1];
};

#define SG_SUBR(obj)      	 ((SgSubr*)(obj))
#define SG_SUBRP(obj)     	 (SG_PROCEDUREP(obj) && SG_PROCEDURE(obj)->type == SG_PROC_SUBR)
#define SG_SUBR_FUNC(obj) 	 (SG_SUBR(obj)->func)
#define SG_SUBR_DATA(obj) 	 (SG_SUBR(obj)->data)
#define SG_SUBR_RETURN_CODE(obj) (SG_SUBR(obj)->returnCode)

#define SG__DEFINE_SUBR_INT(cvar, req, opt, func, inliner, data)	\
  SgSubr cvar = {							\
    SG__PROCEDURE_INITIALIZER(MAKE_HDR_VALUE(TC_PROCEDURE),		\
			      req, opt, SG_PROC_SUBR,			\
			      SG_FALSE, inliner),			\
    (func), (data), {FALSE}						\
  }

#define SG_DEFINE_SUBR(cvar, req, opt, func, inliner, data)	\
  SG__DEFINE_SUBR_INT(cvar, req, opt, func, inliner, data)

#define argumentRef(index, var)			\
  (var) = args[index]

/* convenient macro for cprocedures */
#define castArgumentType(index, tmp, var, required, pred, cast)		\
  argumentRef(index, tmp);						\
  if (!pred(tmp)) {							\
    Sg_WrongTypeOfArgumentViolation(SG_OBJ(procedureName),		\
				    Sg_MakeString(UC(#required),	\
						  SG_LITERAL_STRING),	\
				    tmp,				\
				    SG_NIL);				\
    return SG_UNDEF;							\
  }									\
  var = cast(tmp);

#define argumentAsProcedure(index, tmp, var)				\
  castArgumentType(index, tmp, var, procudure, SG_PROCEDUREP, SG_PROCEDURE)
#define argumentAsPort(index, tmp, var)				\
  castArgumentType(index, tmp, var, port, SG_PORTP, SG_PORT)
#define argumentAsCodeBuilder(index, tmp, var)				\
  castArgumentType(index, tmp, var, code-builder, SG_CODE_BUILDERP, SG_CODE_BUILDER)
#define argumentAsSymbol(index, tmp, var)				\
  castArgumentType(index, tmp, var, symbol, SG_SYMBOLP, SG_SYMBOL)
#define argumentAsHashTable(index, tmp, var)				\
  castArgumentType(index, tmp, var, hashtable, SG_HASHTABLE_P, SG_HASHTABLE)
#define argumentAsIdentifier(index, tmp, var)				\
  castArgumentType(index, tmp, var, identifier, SG_IDENTIFIERP, SG_IDENTIFIER)
#define argumentAsString(index, tmp, var)				\
  castArgumentType(index, tmp, var, string, SG_STRINGP, SG_STRING)
#define argumentAsSyntax(index, tmp, var)				\
  castArgumentType(index, tmp, var, syntax, SG_SYNTAXP, SG_SYNTAX)
#define argumentAsMacro(index, tmp, var)				\
  castArgumentType(index, tmp, var, macro, SG_MACROP, SG_MACRO)
#define argumentAsVector(index, tmp, var)				\
  castArgumentType(index, tmp, var, vector, SG_VECTORP, SG_VECTOR)
#define argumentAsFixnum(index, tmp, var)				\
  castArgumentType(index, tmp, var, fixnum, SG_INTP, SG_INT_VALUE)
#define argumentAsBoolean(index, tmp, var)				\
  castArgumentType(index, tmp, var, boolean, SG_BOOLP, SG_BOOL_VALUE)
#define argumentAsInstance(index, tmp, var)				\
  castArgumentType(index, tmp, var, instance, SG_INSTANCEP, SG_INSTANCE)
#define argumentAsTranscoder(index, tmp, var)				\
  castArgumentType(index, tmp, var, transcoder, SG_TRANSCODERP, SG_TRANSCODER)
#define argumentAsNumber(index, tmp, var)				\
  castArgumentType(index, tmp, var, number, SG_NUMBERP, SG_OBJ)
#define argumentAsGeneric(index, tmp, var)				\
  castArgumentType(index, tmp, var, generic, SG_GENERICP, SG_GENERIC)

/* for library we need special treat */
#define argumentAsLibrary(index, tmp, var)				\
  argumentRef(index, tmp);						\
  if (!SG_LIBRARYP(tmp)) {						\
    var = SG_LIBRARY(Sg_FindLibrary(tmp, FALSE));			\
  } else {								\
    castArgumentType(index, tmp, var, library, SG_LIBRARYP, SG_LIBRARY); \
  }

#define retrieveOptionalArguments(index, var)	\
  var = Sg_ArrayToList(args + (index), argc - (index));

#define DeclareProcedureName(name)					\
  const SgString *procedureName = Sg_MakeString(UC(name), SG_LITERAL_STRING);

#define checkArgumentLength(count)					\
  if (argc != (count)) {						\
    Sg_WrongNumberOfArgumentsViolation(SG_OBJ(procedureName), (count), argc, SG_NIL); \
    return SG_UNDEF;							\
  }
#define checkArgumentLengthBetween(start, end)				\
  if (argc < (start) || argc > (end)) {					\
    Sg_WrongNumberOfArgumentsBetweenViolation(SG_OBJ(procedureName), (start), (end), argc, SG_NIL); \
    return SG_UNDEF;							\
  }
#define checkArgumentLengthAtLeast(count)				\
  if (argc < (count)) {							\
    Sg_WrongNumberOfArgumentsAtLeastViolation(SG_OBJ(procedureName), (count), argc, SG_NIL); \
    return SG_UNDEF;							\
  }
    

SG_CDECL_BEGIN

SG_EXTERN SgObject Sg_MakeSubr(SgSubrProc proc, void *data, int required, int optional, SgObject info);

SG_CDECL_END

#endif /* SAGITTARIUS_SUBR_H_ */

/*
  end of file
  Local Variables:
  coding: utf-8-unix
  End
*/
