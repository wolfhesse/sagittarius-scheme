@; -*- mode: scribble; coding: utf-8; -*-

@subsection[:tag "rnrs.hashtables.6"]{Hashtables}

The @code{(rnrs hashtables (6))}library provides a set of operations on
hashtables. A @var{hashtable} is a data structure that associates keys with
values. Any object can be used as a key, provided a @var{hash function} and a
suitable @var{equivalence function} is available. A hash function is a
procedure that maps keys to exact integer objects. It is the programmer's
responsibility to ensure that the hash function is compatible with the
equivalence function, which is a procedure that accepts two keys and returns
true if they are equivalent and #f otherwise. Standard hashtables for arbitrary
objects based on the @code{eq?} and @code{eqv?} predicates are provided. Also,
hash functions for arbitrary objects, strings, and symbols are provided.

This section uses the @var{hashtable} parameter name for arguments that must be
hashtables, and the @var{key} parameter name for arguments that must be
hashtable keys.

@define[Library]{@name{(rnrs hashtable (6))}}
@desc{[R6RS] This library exports a set of operations on hashtables.}

@subsubsection{Constructors}

@define[Function]{@name{make-eq-hashtable} @args{:optional k weakness}}
@define[Function]{@name{make-eqv-hashtable} @args{:optional k weakness}}
@desc{

[R6RS+] Returns a newly allocated mutable hashtable that accepts arbitrary
objects as keys, and compares those keys with @code{eq?}
(@code{make-eq-hashtable}) or @code{eqv?} (@code{make-eqv-hashtable}).

If optional argument @var{k} is given, it must be exact non-negative integer or
@code{#f}. If it's @code{#f}, then the procedure picks up default initial
capacity, otherwise the initial capacity of the hashtable is set to
approximately @var{k} elements.

If optional argument @var{weakness} is given, then it must be one of the
symbols @code{key}, @code{value} or @code{both}, or @code{#f}. If the value is
one of the symbols, then the procedure creates weak hashtable of given symbol's
weakness. If the symbol is @code{key}, then entries whose keys are refered only
from this hashtable might be removed when garbage collection is
occurred. @code{value} is for entry values. @code{both} is for both.

}

@define[Function]{@name{make-hashtable}
 @args{hash-function equiv :optional k weakness}}
@desc{[R6RS] @var{Hash-function} and @var{equiv} must be procedures. 

@var{Hash-function} should accept a key as an argument and should return a
non-negative exact integer object. @var{Equiv} should accept two keys as
arguments and return a single value.

The @code{make-hashtable} procedure returns a newly allocated mutable hashtable
using @var{hash-function} as the hash function and @var{equiv} as the
equivalence function used to compare keys. 

If optional argument @var{k} and @var{weakness} are the same as
@code{make-eq-hashtable} and @code{make-eqv-hashtable}.

}

@subsubsection{Procedures}

@define[Function]{@name{hashtable?} @args{obj}}
@desc{[R6RS] Returns #t if @var{obj} is a hashtable, #f otherwise.}

@define[Function]{@name{hashtable-size} @args{hashtable}}
@desc{[R6RS] Returns the number of keys contained in @var{hashtable} as an exact
integer object.}

@define[Function]{@name{hashtable-ref}
 @args{hashtable key :optional (default #f)}}
@desc{[R6RS+] Returns the value in @var{hashtable} associated with @var{key}. If
@var{hashtable} does not contain an association for @var{key}, @var{default} is
returned.

Since Sagittarius version 0.3.4, this procedure's @var{default} argument is
optional to implement SRFI-17 efficiently.
}

@define[Function]{@name{hashtable-set!} @args{hashtable key obj}}
@desc{[R6RS] Changes @var{hashtable} to associate @var{key} with @var{obj},
adding a new association or replacing any existing association for @var{key},
and returns unspecified values.
}

@define[Function]{@name{hashtable-delete!} @args{hashtable key}}
@desc{[R6RS] Removes any association for @var{key} within @var{hashtable} and
returns unspecified values.
}

@define[Function]{@name{hashtable-contains?} @args{hashtable key}}
@desc{

[R6RS] Returns #t if @var{hashtable} contains an association for @var{key}, #f
otherwise.

Note: On Sagittarius, @code{hashtable-ref} and @code{hashtable-contains?} do
not make any difference fot the performance.

}

@define[Function]{@name{hashtable-update!} @args{hashtable key proc default}}
desc{[R6RS] @var{Proc} should accept one argument, should return a single
value.

The @code{hashtable-update!} procedure applies @var{proc} to the value in
@var{hashtable} associated with @var{key}, or to @var{default} if
@var{hashtable} does not contain an association for @var{key}. The
@var{hashtable} is then changed to associate @var{key} with the value returned
by @var{proc}.
}

@define[Function]{@name{hashtable-copy} @args{hashtable :optional mutable}}
@desc{[R6RS] Returns a copy of @var{hashtable}. If the @var{mutable} argument
is provided and is true, the returned hashtable is mutable; otherwise it is
immutable.
}

@define[Function]{@name{hashtable-clear} @args{hashtable :optional k}}
@desc{[R6RS] Removes all associations from @var{hashtable} and returns
unspecified values.

If a second argument is given, the current capacity of the hashtable is reset
to approximately @var{k} elements.  }

@define[Function]{@name{hashtable-keys} @args{hashtable}}
@define[Function]{@name{hashtable-entries} @args{hashtable}}
@desc{[R6RS] Returns a vector of all keys or entries in @var{hashtable},
respectively. The order of the vector is unspecified.
}

@subsubsection{Inspection}

@define[Function]{@name{hashtable-equivalence-function} @args{hashtable}}
@define[Function]{@name{hashtable-hash-function} @args{hashtable}}
@desc{[R6RS] Returns the equivalence or hash function used by @var{hashtable}
respectively.
}

@define[Function]{@name{hashtable-mutable?} @args{hashtable}}
@desc{[R6RS] Returns #t if @var{hashtable} is mutable, otherwise #f.}

@subsubsection{Hash functions}

@define[Function]{@name{equal-hash} @args{obj}}
@define[Function]{@name{symbol-hash} @args{symbol}}
@desc{[R6RS] Returns hash value of given argument. Each procedures return
the hash values suitable for @code{equal?} and symbols.
}
@define[Function]{@name{string-hash} @args{string :optional bound start end}}
@define[Function]{@name{string-ci-hash} @args{string :optional bound start end}}
@desc{[R6RS+][SRFI-13] Returns hash value of given argument. Each procedures
 return the hash values suitable for @code{string=?} and @code{string-ci=?}.

If the optional argument @var{start} and @var{end} is given, then the given
string will be substringed with @var{start} and @var{end}.

If the optional argument @var{bound} is given, it must be exact integer and hash
function will also use the given value.
}