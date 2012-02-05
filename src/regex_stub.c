/*Generated by genstub. Do not edit.*/

#line 21 "regex_stub.stub"
#define LIBSAGITTARIUS_BODY


#line 22 "regex_stub.stub"
#include <sagittarius.h>
#include <sagittarius/regex.h>

static struct sg__rcRec {
  SgObject d542[16];
} sg__rc = {
  {  /* SgObject d542 */
    SG_UNBOUND,
    SG_UNBOUND,
    SG_UNBOUND,
    SG_UNBOUND,
    SG_UNBOUND,
    SG_UNBOUND,
    SG_UNBOUND,
    SG_UNBOUND,
    SG_UNBOUND,
    SG_UNBOUND,
    SG_UNBOUND,
    SG_UNBOUND,
    SG_UNBOUND,
    SG_UNBOUND,
    SG_UNBOUND,
    SG_UNBOUND,
  },
};
static SgObject regex_stub_compile_regex(SgObject *SG_FP, int SG_ARGC, void *data_)
{
  SgObject p_scm;
  SgString* p;
  SgObject flags_scm;
  int flags;
  SgObject parse_only_scm;
  int parse_only;
  SG_ENTER_SUBR("compile-regex");
  if (SG_ARGC > 3||
      SG_ARGC < 1)
    Sg_WrongNumberOfArgumentsBetweenViolation(
     SG_INTERN("compile-regex"), 1, 3, SG_ARGC, SG_NIL);
  p_scm = SG_ARGREF(0);
  if (!SG_STRINGP(p_scm))
    Sg_WrongTypeOfArgumentViolation(
       sg__rc.d542[0], SG_MAKE_STRING("string"), p_scm, SG_NIL);
  p = SG_STRING(p_scm);
  if (SG_ARGC >= 1+1) {
  flags_scm = SG_ARGREF(1);
  } else {
  flags_scm = SG_MAKE_INT(0);
  }
  if (!SG_INTP(flags_scm) && !SG_INTP(flags_scm))
    Sg_WrongTypeOfArgumentViolation(
       sg__rc.d542[0], SG_MAKE_STRING("fixnum"), flags_scm, SG_NIL);
  flags = SG_INT_VALUE(flags_scm);
  if (SG_ARGC >= 2+1) {
  parse_only_scm = SG_ARGREF(2);
  } else {
  parse_only_scm = SG_FALSE;
  }
  if (!SG_FALSEP(parse_only_scm) && !SG_BOOLP(parse_only_scm))
    Sg_WrongTypeOfArgumentViolation(
       sg__rc.d542[0], SG_MAKE_STRING("boolean"), parse_only_scm, SG_NIL);
  parse_only = SG_BOOL_VALUE(parse_only_scm);
  {
{
SgObject SG_RESULT = (SgObject)NULL;

#line 27 "regex_stub.stub"
SG_RESULT=(Sg_CompileRegex(p,flags,parse_only));
SG_RETURN(SG_OBJ_SAFE(SG_RESULT));
}
  }
}
static SG_DEFINE_SUBR(regex_stub_compile_regex__STUB, 1, 3,regex_stub_compile_regex, SG_FALSE, NULL);

static SgObject regex_stub_dump_regex(SgObject *SG_FP, int SG_ARGC, void *data_)
{
  SgObject p_scm;
  SgPattern* p;
  SgObject port_scm;
  SgPort* port;
  SG_ENTER_SUBR("dump-regex");
  if (SG_ARGC > 2||
      SG_ARGC < 1)
    Sg_WrongNumberOfArgumentsBetweenViolation(
     SG_INTERN("dump-regex"), 1, 2, SG_ARGC, SG_NIL);
  p_scm = SG_ARGREF(0);
  if (!SG_PATTERNP(p_scm))
    Sg_WrongTypeOfArgumentViolation(
       sg__rc.d542[1], SG_MAKE_STRING("pattern"), p_scm, SG_NIL);
  p = SG_PATTERN(p_scm);
  if (SG_ARGC >= 1+1) {
  port_scm = SG_ARGREF(1);
  } else {
  port_scm = 
#line 30 "regex_stub.stub"
Sg_CurrentOutputPort();;
  }
  if (!SG_PORTP(port_scm))
    Sg_WrongTypeOfArgumentViolation(
       sg__rc.d542[1], SG_MAKE_STRING("port"), port_scm, SG_NIL);
  port = SG_PORT(port_scm);
  {

#line 32 "regex_stub.stub"
Sg_DumpRegex(p,port);
SG_RETURN(SG_UNDEF);
  }
}
static SG_DEFINE_SUBR(regex_stub_dump_regex__STUB, 1, 2,regex_stub_dump_regex, SG_FALSE, NULL);

static SgObject regex_stub_regex_matcher(SgObject *SG_FP, int SG_ARGC, void *data_)
{
  SgObject p_scm;
  SgPattern* p;
  SgObject text_scm;
  SgString* text;
  SG_ENTER_SUBR("regex-matcher");
  if (SG_ARGC != 2)
    Sg_WrongNumberOfArgumentsViolation(
     SG_INTERN("regex-matcher"), 2, SG_ARGC, SG_NIL);
  p_scm = SG_ARGREF(0);
  if (!SG_PATTERNP(p_scm))
    Sg_WrongTypeOfArgumentViolation(
       sg__rc.d542[2], SG_MAKE_STRING("pattern"), p_scm, SG_NIL);
  p = SG_PATTERN(p_scm);
  text_scm = SG_ARGREF(1);
  if (!SG_STRINGP(text_scm))
    Sg_WrongTypeOfArgumentViolation(
       sg__rc.d542[2], SG_MAKE_STRING("string"), text_scm, SG_NIL);
  text = SG_STRING(text_scm);
  {
{
SgObject SG_RESULT = (SgObject)NULL;

#line 36 "regex_stub.stub"
SG_RESULT=(Sg_RegexMatcher(p,text));
SG_RETURN(SG_OBJ_SAFE(SG_RESULT));
}
  }
}
static SG_DEFINE_SUBR(regex_stub_regex_matcher__STUB, 2, 0,regex_stub_regex_matcher, SG_FALSE, NULL);

static SgObject regex_stub_regex_matches(SgObject *SG_FP, int SG_ARGC, void *data_)
{
  SgObject m_scm;
  SgMatcher* m;
  SG_ENTER_SUBR("regex-matches");
  if (SG_ARGC != 1)
    Sg_WrongNumberOfArgumentsViolation(
     SG_INTERN("regex-matches"), 1, SG_ARGC, SG_NIL);
  m_scm = SG_ARGREF(0);
  if (!SG_MATCHERP(m_scm))
    Sg_WrongTypeOfArgumentViolation(
       sg__rc.d542[3], SG_MAKE_STRING("matcher"), m_scm, SG_NIL);
  m = SG_MATCHER(m_scm);
  {
{
int SG_RESULT = (int)NULL;

#line 39 "regex_stub.stub"
SG_RESULT=(Sg_RegexMatches(m));
SG_RETURN(SG_MAKE_BOOL(SG_RESULT));
}
  }
}
static SG_DEFINE_SUBR(regex_stub_regex_matches__STUB, 1, 0,regex_stub_regex_matches, SG_FALSE, NULL);

static SgObject regex_stub_regex_looking_at(SgObject *SG_FP, int SG_ARGC, void *data_)
{
  SgObject m_scm;
  SgMatcher* m;
  SG_ENTER_SUBR("regex-looking-at");
  if (SG_ARGC != 1)
    Sg_WrongNumberOfArgumentsViolation(
     SG_INTERN("regex-looking-at"), 1, SG_ARGC, SG_NIL);
  m_scm = SG_ARGREF(0);
  if (!SG_MATCHERP(m_scm))
    Sg_WrongTypeOfArgumentViolation(
       sg__rc.d542[4], SG_MAKE_STRING("matcher"), m_scm, SG_NIL);
  m = SG_MATCHER(m_scm);
  {
{
int SG_RESULT = (int)NULL;

#line 42 "regex_stub.stub"
SG_RESULT=(Sg_RegexLookingAt(m));
SG_RETURN(SG_MAKE_BOOL(SG_RESULT));
}
  }
}
static SG_DEFINE_SUBR(regex_stub_regex_looking_at__STUB, 1, 0,regex_stub_regex_looking_at, SG_FALSE, NULL);

static SgObject regex_stub_regex_find(SgObject *SG_FP, int SG_ARGC, void *data_)
{
  SgObject m_scm;
  SgMatcher* m;
  SgObject start_scm;
  int start;
  SG_ENTER_SUBR("regex-find");
  if (SG_ARGC > 2||
      SG_ARGC < 1)
    Sg_WrongNumberOfArgumentsBetweenViolation(
     SG_INTERN("regex-find"), 1, 2, SG_ARGC, SG_NIL);
  m_scm = SG_ARGREF(0);
  if (!SG_MATCHERP(m_scm))
    Sg_WrongTypeOfArgumentViolation(
       sg__rc.d542[5], SG_MAKE_STRING("matcher"), m_scm, SG_NIL);
  m = SG_MATCHER(m_scm);
  if (SG_ARGC >= 1+1) {
  start_scm = SG_ARGREF(1);
  } else {
  start_scm = SG_MAKE_INT(-1);
  }
  if (!SG_INTP(start_scm) && !SG_INTP(start_scm))
    Sg_WrongTypeOfArgumentViolation(
       sg__rc.d542[5], SG_MAKE_STRING("fixnum"), start_scm, SG_NIL);
  start = SG_INT_VALUE(start_scm);
  {
{
int SG_RESULT = (int)NULL;

#line 46 "regex_stub.stub"
SG_RESULT=(Sg_RegexFind(m,start));
SG_RETURN(SG_MAKE_BOOL(SG_RESULT));
}
  }
}
static SG_DEFINE_SUBR(regex_stub_regex_find__STUB, 1, 2,regex_stub_regex_find, SG_FALSE, NULL);

static SgObject regex_stub_regex_group(SgObject *SG_FP, int SG_ARGC, void *data_)
{
  SgObject m_scm;
  SgMatcher* m;
  SgObject group_scm;
  int group;
  SG_ENTER_SUBR("regex-group");
  if (SG_ARGC > 2||
      SG_ARGC < 1)
    Sg_WrongNumberOfArgumentsBetweenViolation(
     SG_INTERN("regex-group"), 1, 2, SG_ARGC, SG_NIL);
  m_scm = SG_ARGREF(0);
  if (!SG_MATCHERP(m_scm))
    Sg_WrongTypeOfArgumentViolation(
       sg__rc.d542[6], SG_MAKE_STRING("matcher"), m_scm, SG_NIL);
  m = SG_MATCHER(m_scm);
  if (SG_ARGC >= 1+1) {
  group_scm = SG_ARGREF(1);
  } else {
  group_scm = SG_MAKE_INT(0);
  }
  if (!SG_INTP(group_scm) && !SG_INTP(group_scm))
    Sg_WrongTypeOfArgumentViolation(
       sg__rc.d542[6], SG_MAKE_STRING("fixnum"), group_scm, SG_NIL);
  group = SG_INT_VALUE(group_scm);
  {
{
SgObject SG_RESULT = (SgObject)NULL;

#line 49 "regex_stub.stub"
SG_RESULT=(Sg_RegexGroup(m,group));
SG_RETURN(SG_OBJ_SAFE(SG_RESULT));
}
  }
}
static SG_DEFINE_SUBR(regex_stub_regex_group__STUB, 1, 2,regex_stub_regex_group, SG_FALSE, NULL);

static SgObject regex_stub_regex_after(SgObject *SG_FP, int SG_ARGC, void *data_)
{
  SgObject m_scm;
  SgMatcher* m;
  SG_ENTER_SUBR("regex-after");
  if (SG_ARGC != 1)
    Sg_WrongNumberOfArgumentsViolation(
     SG_INTERN("regex-after"), 1, SG_ARGC, SG_NIL);
  m_scm = SG_ARGREF(0);
  if (!SG_MATCHERP(m_scm))
    Sg_WrongTypeOfArgumentViolation(
       sg__rc.d542[7], SG_MAKE_STRING("matcher"), m_scm, SG_NIL);
  m = SG_MATCHER(m_scm);
  {
{
SgObject SG_RESULT = (SgObject)NULL;

#line 52 "regex_stub.stub"
SG_RESULT=(Sg_Substring((m)->text,
(m)->last,
(m)->to));
SG_RETURN(SG_OBJ_SAFE(SG_RESULT));
}
  }
}
static SG_DEFINE_SUBR(regex_stub_regex_after__STUB, 1, 0,regex_stub_regex_after, SG_FALSE, NULL);

static SgObject regex_stub_regex_before(SgObject *SG_FP, int SG_ARGC, void *data_)
{
  SgObject m_scm;
  SgMatcher* m;
  SG_ENTER_SUBR("regex-before");
  if (SG_ARGC != 1)
    Sg_WrongNumberOfArgumentsViolation(
     SG_INTERN("regex-before"), 1, SG_ARGC, SG_NIL);
  m_scm = SG_ARGREF(0);
  if (!SG_MATCHERP(m_scm))
    Sg_WrongTypeOfArgumentViolation(
       sg__rc.d542[8], SG_MAKE_STRING("matcher"), m_scm, SG_NIL);
  m = SG_MATCHER(m_scm);
  {
{
SgObject SG_RESULT = (SgObject)NULL;

#line 57 "regex_stub.stub"
SG_RESULT=(Sg_Substring((m)->text,0,
#line 59 "regex_stub.stub"
(m)->first));
SG_RETURN(SG_OBJ_SAFE(SG_RESULT));
}
  }
}
static SG_DEFINE_SUBR(regex_stub_regex_before__STUB, 1, 0,regex_stub_regex_before, SG_FALSE, NULL);

static SgObject regex_stub_regex_capture_count(SgObject *SG_FP, int SG_ARGC, void *data_)
{
  SgObject m_scm;
  SgMatcher* m;
  SG_ENTER_SUBR("regex-capture-count");
  if (SG_ARGC != 1)
    Sg_WrongNumberOfArgumentsViolation(
     SG_INTERN("regex-capture-count"), 1, SG_ARGC, SG_NIL);
  m_scm = SG_ARGREF(0);
  if (!SG_MATCHERP(m_scm))
    Sg_WrongTypeOfArgumentViolation(
       sg__rc.d542[9], SG_MAKE_STRING("matcher"), m_scm, SG_NIL);
  m = SG_MATCHER(m_scm);
  {
{
int SG_RESULT = (int)NULL;

#line 62 "regex_stub.stub"
SG_RESULT=(Sg_RegexCaptureCount(m));
SG_RETURN(SG_MAKE_INT(SG_RESULT));
}
  }
}
static SG_DEFINE_SUBR(regex_stub_regex_capture_count__STUB, 1, 0,regex_stub_regex_capture_count, SG_FALSE, NULL);

static SgObject regex_stub_regex_first(SgObject *SG_FP, int SG_ARGC, void *data_)
{
  SgObject m_scm;
  SgMatcher* m;
  SG_ENTER_SUBR("regex-first");
  if (SG_ARGC != 1)
    Sg_WrongNumberOfArgumentsViolation(
     SG_INTERN("regex-first"), 1, SG_ARGC, SG_NIL);
  m_scm = SG_ARGREF(0);
  if (!SG_MATCHERP(m_scm))
    Sg_WrongTypeOfArgumentViolation(
       sg__rc.d542[10], SG_MAKE_STRING("matcher"), m_scm, SG_NIL);
  m = SG_MATCHER(m_scm);
  {
{
int SG_RESULT = (int)NULL;

#line 65 "regex_stub.stub"
SG_RESULT=((m)->first);
SG_RETURN(SG_MAKE_INT(SG_RESULT));
}
  }
}
static SG_DEFINE_SUBR(regex_stub_regex_first__STUB, 1, 0,regex_stub_regex_first, SG_FALSE, NULL);

static SgObject regex_stub_regex_last(SgObject *SG_FP, int SG_ARGC, void *data_)
{
  SgObject m_scm;
  SgMatcher* m;
  SG_ENTER_SUBR("regex-last");
  if (SG_ARGC != 1)
    Sg_WrongNumberOfArgumentsViolation(
     SG_INTERN("regex-last"), 1, SG_ARGC, SG_NIL);
  m_scm = SG_ARGREF(0);
  if (!SG_MATCHERP(m_scm))
    Sg_WrongTypeOfArgumentViolation(
       sg__rc.d542[11], SG_MAKE_STRING("matcher"), m_scm, SG_NIL);
  m = SG_MATCHER(m_scm);
  {
{
int SG_RESULT = (int)NULL;

#line 68 "regex_stub.stub"
SG_RESULT=((m)->last);
SG_RETURN(SG_MAKE_INT(SG_RESULT));
}
  }
}
static SG_DEFINE_SUBR(regex_stub_regex_last__STUB, 1, 0,regex_stub_regex_last, SG_FALSE, NULL);

static SgObject regex_stub_regex_replace_all(SgObject *SG_FP, int SG_ARGC, void *data_)
{
  SgObject m_scm;
  SgMatcher* m;
  SgObject replacement_scm;
  SgString* replacement;
  SG_ENTER_SUBR("regex-replace-all");
  if (SG_ARGC != 2)
    Sg_WrongNumberOfArgumentsViolation(
     SG_INTERN("regex-replace-all"), 2, SG_ARGC, SG_NIL);
  m_scm = SG_ARGREF(0);
  if (!SG_MATCHERP(m_scm))
    Sg_WrongTypeOfArgumentViolation(
       sg__rc.d542[12], SG_MAKE_STRING("matcher"), m_scm, SG_NIL);
  m = SG_MATCHER(m_scm);
  replacement_scm = SG_ARGREF(1);
  if (!SG_STRINGP(replacement_scm))
    Sg_WrongTypeOfArgumentViolation(
       sg__rc.d542[12], SG_MAKE_STRING("string"), replacement_scm, SG_NIL);
  replacement = SG_STRING(replacement_scm);
  {
{
SgObject SG_RESULT = (SgObject)NULL;

#line 71 "regex_stub.stub"
SG_RESULT=(Sg_RegexReplaceAll(m,replacement));
SG_RETURN(SG_OBJ_SAFE(SG_RESULT));
}
  }
}
static SG_DEFINE_SUBR(regex_stub_regex_replace_all__STUB, 2, 0,regex_stub_regex_replace_all, SG_FALSE, NULL);

static SgObject regex_stub_regex_replace_first(SgObject *SG_FP, int SG_ARGC, void *data_)
{
  SgObject m_scm;
  SgMatcher* m;
  SgObject replacement_scm;
  SgString* replacement;
  SG_ENTER_SUBR("regex-replace-first");
  if (SG_ARGC != 2)
    Sg_WrongNumberOfArgumentsViolation(
     SG_INTERN("regex-replace-first"), 2, SG_ARGC, SG_NIL);
  m_scm = SG_ARGREF(0);
  if (!SG_MATCHERP(m_scm))
    Sg_WrongTypeOfArgumentViolation(
       sg__rc.d542[13], SG_MAKE_STRING("matcher"), m_scm, SG_NIL);
  m = SG_MATCHER(m_scm);
  replacement_scm = SG_ARGREF(1);
  if (!SG_STRINGP(replacement_scm))
    Sg_WrongTypeOfArgumentViolation(
       sg__rc.d542[13], SG_MAKE_STRING("string"), replacement_scm, SG_NIL);
  replacement = SG_STRING(replacement_scm);
  {
{
SgObject SG_RESULT = (SgObject)NULL;

#line 74 "regex_stub.stub"
SG_RESULT=(Sg_RegexReplaceFirst(m,replacement));
SG_RETURN(SG_OBJ_SAFE(SG_RESULT));
}
  }
}
static SG_DEFINE_SUBR(regex_stub_regex_replace_first__STUB, 2, 0,regex_stub_regex_replace_first, SG_FALSE, NULL);

static SgObject regex_stub_regex_patternP(SgObject *SG_FP, int SG_ARGC, void *data_)
{
  SgObject o_scm;
  SgObject o;
  SG_ENTER_SUBR("regex-pattern?");
  if (SG_ARGC != 1)
    Sg_WrongNumberOfArgumentsViolation(
     SG_INTERN("regex-pattern?"), 1, SG_ARGC, SG_NIL);
  o_scm = SG_ARGREF(0);
  o = (o_scm);
  {
{
int SG_RESULT = (int)NULL;

#line 78 "regex_stub.stub"
SG_RESULT=(SG_PATTERNP(o));
SG_RETURN(SG_MAKE_BOOL(SG_RESULT));
}
  }
}
static SG_DEFINE_SUBR(regex_stub_regex_patternP__STUB, 1, 0,regex_stub_regex_patternP, SG_FALSE, NULL);

static SgObject regex_stub_regex_matcherP(SgObject *SG_FP, int SG_ARGC, void *data_)
{
  SgObject o_scm;
  SgObject o;
  SG_ENTER_SUBR("regex-matcher?");
  if (SG_ARGC != 1)
    Sg_WrongNumberOfArgumentsViolation(
     SG_INTERN("regex-matcher?"), 1, SG_ARGC, SG_NIL);
  o_scm = SG_ARGREF(0);
  o = (o_scm);
  {
{
int SG_RESULT = (int)NULL;

#line 81 "regex_stub.stub"
SG_RESULT=(SG_MATCHERP(o));
SG_RETURN(SG_MAKE_BOOL(SG_RESULT));
}
  }
}
static SG_DEFINE_SUBR(regex_stub_regex_matcherP__STUB, 1, 0,regex_stub_regex_matcherP, SG_FALSE, NULL);

void Sg__Init_sagittarius_regex_impl () {
  SgLibrary *lib = SG_LIBRARY(Sg_FindLibrary(SG_INTERN("(sagittarius regex impl)"), TRUE));
  sg__rc.d542[0] = SG_MAKE_STRING("compile-regex");
  Sg_InsertBinding(lib, Sg_Intern(sg__rc.d542[0]), &regex_stub_compile_regex__STUB);
  SG_PROCEDURE_NAME(&regex_stub_compile_regex__STUB) = sg__rc.d542[0];
  sg__rc.d542[1] = SG_MAKE_STRING("dump-regex");
  Sg_InsertBinding(lib, Sg_Intern(sg__rc.d542[1]), &regex_stub_dump_regex__STUB);
  SG_PROCEDURE_NAME(&regex_stub_dump_regex__STUB) = sg__rc.d542[1];
  sg__rc.d542[2] = SG_MAKE_STRING("regex-matcher");
  Sg_InsertBinding(lib, Sg_Intern(sg__rc.d542[2]), &regex_stub_regex_matcher__STUB);
  SG_PROCEDURE_NAME(&regex_stub_regex_matcher__STUB) = sg__rc.d542[2];
  sg__rc.d542[3] = SG_MAKE_STRING("regex-matches");
  Sg_InsertBinding(lib, Sg_Intern(sg__rc.d542[3]), &regex_stub_regex_matches__STUB);
  SG_PROCEDURE_NAME(&regex_stub_regex_matches__STUB) = sg__rc.d542[3];
  sg__rc.d542[4] = SG_MAKE_STRING("regex-looking-at");
  Sg_InsertBinding(lib, Sg_Intern(sg__rc.d542[4]), &regex_stub_regex_looking_at__STUB);
  SG_PROCEDURE_NAME(&regex_stub_regex_looking_at__STUB) = sg__rc.d542[4];
  sg__rc.d542[5] = SG_MAKE_STRING("regex-find");
  Sg_InsertBinding(lib, Sg_Intern(sg__rc.d542[5]), &regex_stub_regex_find__STUB);
  SG_PROCEDURE_NAME(&regex_stub_regex_find__STUB) = sg__rc.d542[5];
  sg__rc.d542[6] = SG_MAKE_STRING("regex-group");
  Sg_InsertBinding(lib, Sg_Intern(sg__rc.d542[6]), &regex_stub_regex_group__STUB);
  SG_PROCEDURE_NAME(&regex_stub_regex_group__STUB) = sg__rc.d542[6];
  sg__rc.d542[7] = SG_MAKE_STRING("regex-after");
  Sg_InsertBinding(lib, Sg_Intern(sg__rc.d542[7]), &regex_stub_regex_after__STUB);
  SG_PROCEDURE_NAME(&regex_stub_regex_after__STUB) = sg__rc.d542[7];
  sg__rc.d542[8] = SG_MAKE_STRING("regex-before");
  Sg_InsertBinding(lib, Sg_Intern(sg__rc.d542[8]), &regex_stub_regex_before__STUB);
  SG_PROCEDURE_NAME(&regex_stub_regex_before__STUB) = sg__rc.d542[8];
  sg__rc.d542[9] = SG_MAKE_STRING("regex-capture-count");
  Sg_InsertBinding(lib, Sg_Intern(sg__rc.d542[9]), &regex_stub_regex_capture_count__STUB);
  SG_PROCEDURE_NAME(&regex_stub_regex_capture_count__STUB) = sg__rc.d542[9];
  sg__rc.d542[10] = SG_MAKE_STRING("regex-first");
  Sg_InsertBinding(lib, Sg_Intern(sg__rc.d542[10]), &regex_stub_regex_first__STUB);
  SG_PROCEDURE_NAME(&regex_stub_regex_first__STUB) = sg__rc.d542[10];
  sg__rc.d542[11] = SG_MAKE_STRING("regex-last");
  Sg_InsertBinding(lib, Sg_Intern(sg__rc.d542[11]), &regex_stub_regex_last__STUB);
  SG_PROCEDURE_NAME(&regex_stub_regex_last__STUB) = sg__rc.d542[11];
  sg__rc.d542[12] = SG_MAKE_STRING("regex-replace-all");
  Sg_InsertBinding(lib, Sg_Intern(sg__rc.d542[12]), &regex_stub_regex_replace_all__STUB);
  SG_PROCEDURE_NAME(&regex_stub_regex_replace_all__STUB) = sg__rc.d542[12];
  sg__rc.d542[13] = SG_MAKE_STRING("regex-replace-first");
  Sg_InsertBinding(lib, Sg_Intern(sg__rc.d542[13]), &regex_stub_regex_replace_first__STUB);
  SG_PROCEDURE_NAME(&regex_stub_regex_replace_first__STUB) = sg__rc.d542[13];
  sg__rc.d542[14] = SG_MAKE_STRING("regex-pattern?");
  Sg_InsertBinding(lib, Sg_Intern(sg__rc.d542[14]), &regex_stub_regex_patternP__STUB);
  SG_PROCEDURE_NAME(&regex_stub_regex_patternP__STUB) = sg__rc.d542[14];
  sg__rc.d542[15] = SG_MAKE_STRING("regex-matcher?");
  Sg_InsertBinding(lib, Sg_Intern(sg__rc.d542[15]), &regex_stub_regex_matcherP__STUB);
  SG_PROCEDURE_NAME(&regex_stub_regex_matcherP__STUB) = sg__rc.d542[15];
}
