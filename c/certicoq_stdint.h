/* This file declares stdint functionality depending on
 * whether this file is compiled for Linux Kernel
 * or regular user space programs.
 * This file is necessary when using size_t or uint8_t,
 * for example.
 * NOTE: copied from https://github.com/eladrion/certicoq/blob/fphd/plugin/runtime/certicoq_stdint.h
 */

#ifndef CERTICOQ_STDINT_H
#define CERTICOQ_STDINT_H

#ifdef CLIGHT_KERNEL_CODE
 // Definitions for use when compiling for Linux Kernel modules
 #include <linux/types.h>
#else // !CLIGHT_KERNEL_CODE
 // For userland code, load the libc stdint.
 #include <stdint.h>
#endif // CLIGHT_KERNEL_CODE

#endif // CERTICOQ_STDINT_H