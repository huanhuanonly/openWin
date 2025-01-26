#define _Win_Begin_ \
    ::ErrorStream* const _L_currentErrorStream = this->_M_errorStream; \
    ::ErrorStreamGuard _L_errorStreamGuard(*_L_currentErrorStream, __func__);

#define _Win_Static_Begin_ \
    ::ErrorStream* const _L_currentErrorStream = ::ErrorStream::global(); \
    ::ErrorStreamGuard _L_errorStreamGuard(*ErrorStream::global(), __func__);

#define _Win_Check_ \
    do { if (_L_currentErrorStream->check() == false) { return; } } while (false);

#define _Win_Check_with_(...) \
    do { if (_L_currentErrorStream->check() == false) { return __VA_ARGS__; } } while (false);

#define _Win_Check_Noreturn_ \
    _L_currentErrorStream->check();

#define _Win_Failed_ \
    do { _L_currentErrorStream->setFail(); return; } while (false);

#define _Win_Failed_with_(...) \
    do { _L_currentErrorStream->setFail(); return __VA_ARGS__; } while (false);

#define _Win_Failed_Noreturn_ \
    _L_currentErrorStream->setFail();

#define _Win_Test_(ret, ...) \
    do { if (not(ret)) { _Win_Failed_with_(__VA_ARGS__); } } while (false);

#define _Win_Return_on_failed_ \
    do { if (_L_currentErrorStream->failed()) { return; } } while (false);

#define _Win_Return_on_failed_with_(...) \
    do { if (_L_currentErrorStream->failed()) { return __VA_ARGS__; } } while (false);
