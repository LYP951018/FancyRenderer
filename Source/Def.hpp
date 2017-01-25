#pragma once


#define DefGetter(name, type, ret) \
    auto name() const /*noexcept*/ -> type \
    { return ret; }

#define DefSetter(name, type, member) \
    void name(type value) /*noexcept*/ \
    { member = value; }


#ifdef __cpp_inline_variables
#define DefConstInlineVar(type, name, ...) \
    inline const type name = __VA_ARGS__
#else
#define DefConstInlineVar(type, name, ...) \
    extern const __declspec(selectany) type name = __VA_ARGS__
#endif