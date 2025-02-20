#undef __FUNCTION_PROTOTYPE__

#if defined(_MSC_VER)
#   define __FUNCTION_PROTOTYPE__ __FUNCSIG__
#elif defined(__GNUC__) or defined(__clang__)
#   define __FUNCTION_PROTOTYPE__ __PRETTY_FUNCTION__
#else
#   define __FUNCTION_PROTOTYPE__ __func__
#endif

#define _Win_Begin_ \
    ::ErrorStream* const _L_currentErrorStream = this->_M_errorStream; \
    ::ErrorStreamGuard _L_errorStreamGuard(*_L_currentErrorStream, __FUNCTION_PROTOTYPE__);

#define _Win_Begin_Nocheck_ \
    ::ErrorStream* const _L_currentErrorStream = this->_M_errorStream; \
    ::ErrorStreamGuard _L_errorStreamGuard(*_L_currentErrorStream, __FUNCTION_PROTOTYPE__, false);

#define _Win_Static_Begin_ \
    ::ErrorStream* const _L_currentErrorStream = ::ErrorStream::global(); \
    ::ErrorStreamGuard _L_errorStreamGuard(*_L_currentErrorStream, __FUNCTION_PROTOTYPE__);

#define _Win_Static_Begin_Nocheck_ \
    ::ErrorStream* const _L_currentErrorStream = ::ErrorStream::global(); \
    ::ErrorStreamGuard _L_errorStreamGuard(*_L_currentErrorStream, __FUNCTION_PROTOTYPE__, false);


#define _Win_Return_Nocheck_ \
    do { _L_errorStreamGuard.skipCheck(); return; } while (false);

#define _Win_Return_Nocheck_with_(...) \
    do { _L_errorStreamGuard.skipCheck(); return __VA_ARGS__; } while (false);


#define _Win_Check_ \
    do { if (not _L_currentErrorStream->check()) { _Win_Return_Nocheck_ } } while (false);

#define _Win_Check_with_(...) \
    do { if (not _L_currentErrorStream->check()) { _Win_Return_Nocheck_with_(__VA_ARGS__); } } while (false);

#define _Win_Check_Noreturn_ \
    void (_L_currentErrorStream->check());


#define _Win_Failed_ \
    do { _L_currentErrorStream->setFail(); _Win_Return_Nocheck_ } while (false);

#define _Win_Failed_with_(...) \
    do { _L_currentErrorStream->setFail(); _Win_Return_Nocheck_with_(__VA_ARGS__) } while (false);

#define _Win_Failed_Noreturn_ \
    _L_currentErrorStream->setFail();


#define _Win_Test_(ret, ...) \
    do { if (not (ret)) { _Win_Failed_with_(__VA_ARGS__); } } while (false);


#define _Win_Return_on_failed_ \
    do { if (_L_currentErrorStream->failed()) { _Win_Return_Nocheck_ } } while (false);

#define _Win_Return_on_failed_with_(...) \
    do { if (_L_currentErrorStream->failed()) { _Win_Return_Nocheck_with_(__VA_ARGS__) } } while (false);
