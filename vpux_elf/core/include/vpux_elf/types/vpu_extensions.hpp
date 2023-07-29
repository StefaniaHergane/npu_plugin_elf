//
// Copyright (C) 2023 Intel Corporation
// SPDX-License-Identifier: Apache 2.0
//

//

#pragma once

#include <vpux_elf/types/data_types.hpp>
#include <vpux_elf/types/symbol_entry.hpp>
#include <vpux_elf/types/section_header.hpp>

namespace elf {

//
// Relocation types
//

/// Originated from DMA src and sdt field
/// It is used also for MappedInference fields (variants, invariants, dma, barriers) relocations
/// Formula: Dst = S + A
constexpr Elf_Word R_VPU_64 = 0;

/// Originated from link_address relocation in DMA in case of linking to DDR
/// Formula: Dst |= S + A
constexpr Elf_Word R_VPU_64_OR = 1;

/// Originated from link_address relocation in DMA in case of linking to CMX slot
/// Formula:
/// 1) oldDst = Dst & 0xffffffffff
/// 2) Dst &= 0xffffff0000000000
/// 3) Dst |= (S + (A * (oldDst & (Z - 1)))) & 0xffffffffff
/// A - contains the size of element being relocated
/// Dst - contains an index of CMX slot
/// Z - the size of CMX locator
constexpr Elf_Word R_VPU_DISP40_RTM = 2;

/// Originated from barriers relocation
/// Formula: Dst <<= S
constexpr Elf_Word R_VPU_64_LSHIFT = 3;

/// The same as R_VPU_64 but applied for the target address being reinterpreted as uint32_t*
constexpr Elf_Word R_VPU_32 = 4;

/// Originated from invariant_addr field of variant
/// Formula: Dst = (S + (A * (Dst & (Z - 1))))
/// A - contains the size of element being relocated
/// Dst - contains an index of CMX slot
/// Z - the size of CMX locator
constexpr Elf_Word R_VPU_32_RTM = 5;

/// Originated from weight_table_offset field of variant
/// Formula: Dst += S + A
constexpr Elf_Word R_VPU_32_SUM = 6;

/// Originated from pt_base and sp_base fields of invariant
/// Formula: Dst = to_dpu_multicast_base(S + A)
constexpr Elf_Word R_VPU_32_MULTICAST_BASE = 7;

/// Originated from base_adr field of invariant
/// Formula: Dst = to_dpu_multicast_base(S + A) - Dst
constexpr Elf_Word R_VPU_32_MULTICAST_BASE_SUB = 8;

/// Originated from cast_offset field of invariant
/// Formula:
/// 1) oldDst = Dst >> 4
/// 2) Dst &= 0xf
/// 3) Dst |= offs[oldDst] << 4
/// Where offs we get from:
///     unsigned int offs[3] = {SLICE_LENGTH >> 4, SLICE_LENGTH >> 4, SLICE_LENGTH >> 4};
///     to_dpu_multicast(S + A, offs[0], offs[1], offs[2]);
///     Where SLICE_LENGTH = 2 * 1024 * 1024
constexpr Elf_Word R_VPU_DISP28_MULTICAST_OFFSET = 9;

/// Originated from cast_enable field of invariant
/// Formula:
/// 1) oldDst = Dst & 0xf
/// 2) Dst &= 0xfffffff0
/// 3) Dst |= offs[oldDst] & 0xf
/// Where offs we get from:
///     unsigned int offs[3] = {SLICE_LENGTH >> 4, SLICE_LENGTH >> 4, SLICE_LENGTH >> 4};
///     to_dpu_multicast(S + A, offs[0], offs[1], offs[2]);
///     Where SLICE_LENGTH = 2 * 1024 * 1024
constexpr Elf_Word R_VPU_DISP4_MULTICAST_OFFSET_CMP = 10;

// Originated from act_offset field of DPU invariant
// Formula:
// Dst = (S + A) & LO_21_BIT_MASK
constexpr Elf_Word R_VPU_LO_21 = 11;

// Originated from weight_start field of DPU variant
// Formula:
// Dst += (S + A) & LO_21_BIT_MASK
constexpr Elf_Word R_VPU_LO_21_SUM = 12;

// Originated from sp_base field of DPU invariant
// Formula:
// Dst = to_dpu_multicast_base((S + A) & LO_21_BIT_MASK)
constexpr Elf_Word R_VPU_LO_21_MULTICAST_BASE = 13;

// Special Reloc for Variant to Invariant link
// Formula:
// Dst = ((S + A) & 0x0001'FFFF) >> 5
constexpr Elf_Word R_VPU_LO_17_RSHIFT_5 = 14;

//
// Symbol types
//

constexpr uint8_t VPU_STT_ENTRY = STT_LOOS;

//
// Section types
//

constexpr Elf_Word VPU_SHT_NETDESC   = 0x8aaaaaaa;
constexpr Elf_Word VPU_SHT_PROF      = 0x8aaaaaab;
//
// Section flags
//

constexpr Elf_Xword VPU_SHF_JIT             = 0x100000;
constexpr Elf_Xword VPU_SHF_USERINPUT       = 0x200000;
constexpr Elf_Xword VPU_SHF_USEROUTPUT      = 0x400000;
constexpr Elf_Xword VPU_SHF_PROFOUTPUT      = 0x800000;
constexpr Elf_Xword VPU_SHF_PROC_DPU        = 0x10000000;
constexpr Elf_Xword VPU_SHF_PROC_DMA        = 0x20000000;
constexpr Elf_Xword VPU_SHF_PROC_SHAVE      = 0x40000000;

//
// Special section indexes
//

constexpr Elf_Word VPU_RT_SYMTAB = SHN_LOOS;

//
// Run-time owned symtab indexes
//

constexpr Elf_Word VPU_NNRD_SYM_NNCXM_SLICE_BASE_ADDR = 0;
constexpr Elf_Word VPU_NNRD_SYM_RTM_IVAR = 1;
constexpr Elf_Word VPU_NNRD_SYM_RTM_ACT = 2;
constexpr Elf_Word VPU_NNRD_SYM_RTM_DMA0 = 3;
constexpr Elf_Word VPU_NNRD_SYM_RTM_DMA1 = 4;
constexpr Elf_Word VPU_NNRD_SYM_FIFO_BASE = 5;
constexpr Elf_Word VPU_NNRD_SYM_BARRIERS_START = 6;
constexpr Elf_Word VPU_NNRD_SYM_HW_REGISTER = 7;

//
// VPU constants
//

const Elf_Word VPU_SH_INFO_FOR_VPU = 0;
const Elf_Word VPU_SH_ADDR_ALIGN_FOR_VPU = 64;

}
