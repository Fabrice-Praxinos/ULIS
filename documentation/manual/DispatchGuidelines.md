# Dispatch Guidelines
> ULIS uses a mechanism for dispatching implementations of image processing algorithms according to various factors.

---

## Table of Contents
- [Overview](@ref overview)
    + [Compile Time Intrinsic Set Support](@ref compile-time-intrinsic-set-support)
    + [Runtime Host Device Features](@ref runtime-host-device-features)
    + [Runtime Performance Intents](@ref runtime-performance-intents)
    + [Image Format Specializations](@ref image-format-specializations)
    + [Multithreaded Scheduling Options](@ref multithreaded-scheduling-options)
    + [Implementation Variants](@ref implementation-variants)
    + [CPU vs GPU](@ref cpu-vs-gpu)
- [Details](@ref details)

---

## Overview {#overview}

##### Compile Time Intrinsic Set Support {#compile-time-intrinsic-set-support}
- Is the CPU that compiles the code 64-bit ?
- Does the CPU that compiles the code support AVX2 or SSE4 ?

> The machine that compiles the ULIS library might have support for AVX2, but it is not guaranteed that the computer that will run the compiled code once deployed and distributed in a software release has support for this particular intrinsic set. Running compiled code for unsupported intrinsic set could raise "illegal instruction" errors.  
> This is a different problem than what libraries like [simde](https://github.com/simd-everywhere/simde) or [VCL](https://github.com/vectorclass/version1) adress. These libraries typically make it easy to fallback to the appropriate intrinsic set by wrapping the SIMD extension calls, since the fallback occurs at compile-time. They still may compile down to a higher intrinsic set than what the consumer computer is able to support.  
> ULIS provides tools to detect the supported intrinsic sets at compile time.

##### Runtime Host Device Features {#runtime-host-device-features}
- Is the CPU that runs the code 64-bit ?
- Does the CPU that runs the code support AVX2 or SSE4 ?

> This goes hand to hand with the previous concern, but it leverages its own mechanics in order to work as expected.  
> Fortunately, ULIS provides tools to detect the supported intrinsic sets at compile time, with the help of the FDevice class.

##### Runtime Performance Intents {#runtime-performance-intents}
At runtime, we may want to select an implementation using a lower intrinsic set than the maximum supported, for testing purposes.

> This goes hand to hand with the previous concern: with the help of the runtime device features support detection tools, we can select a specific intrinsic set while debugging and checking the implementation will work on computers with lower features support.  
> This is usually specified when creating a instance of the FContext class.

##### Image Format Specializations {#image-format-specializations}
At runtime, the dispatch sometimes selects a dedicated separate implementation that is more efficient according to the format.

> This goes hand to hand with the preious issue, and is usually done automatically when creating a instance of the FContext class. The performance instance might hint to select the generic fallback, or the appropriate specialization.

##### Multithreaded Scheduling Options {#multithreaded-scheduling-options}
Some scheduling strategies need different implementations, a mono-threading implementation is sometimes different from a multi-threaded scanline approach.

> This is transparent for the caller but allows to select the most efficient implementation with assumptions, without making generic versions that will have overheads most of the time.  
> For the user, the selection is not explicit but is computed from the FSchedulePolicy inputs.  

##### Implementation Variants {#implementation-variants}
Some algorithms are very similar and provides a unique entry point, but the actual underlying implementation may differ based on an input enum, for example.

> For example, the Blend entry points provide a common interface for all blending modes, but the implementation is separated according to the blending mode qualifier, wether it's a separable or a non-separable blending mode.

##### CPU vs GPU {#cpu-vs-gpu}
Some algorithms may be dispatched on the CPU or on the GPU depending on the nature of the problem.

> Most of this is made explicit to the end user through named entry points though.  
> Most of the points made here apply to methods dispatched on CPU, but some dispatch mechanisms apply to GPU implementations too.  

---

## Details {#details}