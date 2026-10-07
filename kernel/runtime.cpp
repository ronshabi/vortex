using TCtorFn = void(*)();
extern "C"  TCtorFn __init_array_start[];
extern "C"  TCtorFn __init_array_end[];

// Call all C++ global constructors using the values we defined with
// the linker.

extern "C" void cxx_run_global_ctors(void)
{
    for (TCtorFn *fn = __init_array_start; fn != __init_array_end; ++fn) {
        (*fn)();
    }
}

// The kernel will never exit so I don't care about global dtors
extern "C" {
    void *__dso_handle = nullptr;
    int __cxa_atexit(void(*)(void *), void *, void *)
    {
        return 0;
    }

    int __cxa_pure_virtual(void)
    {
        // We should panic here, but I should prepare an exception info for that
        // For now just halt
        for (;;) {}
    }
}