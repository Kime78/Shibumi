#include "mmu.hpp"
#include <fstream>

uint32_t MMU::virt_to_phys(uint64_t virt)
{
    if ((virt >= 0x80000000 && virt <= 0x9FFFFFFF) | (virt >= 0xA0000000 && virt <= 0xBFFFFFFF))
    {
        return virtual_to_physical_direct(virt);
    }
    else
    {
        return virtual_to_physical_tbl(virt);
    }
}

uint32_t MMU::virtual_to_physical_direct(uint64_t virt)
{
    if (virt >= 0x80000000 && virt <= 0x9FFFFFFF) //KSEG0
    {
        return virt - 0x80000000;
    }
    else if (virt >= 0xA0000000 && virt <= 0xBFFFFFFF) //KSEG1
    {
        return virt - 0xA0000000;
    }
}
uint32_t MMU::virtual_to_physical_tbl(uint64_t virt)
{
}
uint64_t MMU::read64(uint64_t addr)
{
    uint64_t phys = virt_to_phys(addr);
    if (phys >= 0x00000000 && phys <= 0x007FFFFF)
    {
        return rdram[phys] << 56 | rdram[phys + 1] << 48 | rdram[phys + 2] << 40 |
               rdram[phys + 3] << 32 | rdram[phys + 4] << 24 | rdram[phys + 5] << 16 | rdram[phys + 6] << 8 | rdram[phys + 7];
    }

    else if (phys >= 0x03F00000 && phys <= 0x03FFFFFF)
    {
        return rdram_regs[phys - 0x03F00000] << 56 | rdram_regs[phys - 0x03F00000 + 1] << 48 | rdram_regs[phys - 0x03F00000 + 2] << 40 |
               rdram_regs[phys - 0x03F00000 + 3] << 32 | rdram_regs[phys - 0x03F00000 + 4] << 24 | rdram_regs[phys - 0x03F00000 + 5] << 16 |
               rdram_regs[phys - 0x03F00000 + 6] << 8 | rdram_regs[phys - 0x03F00000 + 7];
    }

    else if (phys >= 0x04000000 && phys <= 0x04000FFF)
    {
        return sp_dmem[phys - 0x04000000] << 56 | sp_dmem[phys - 0x04000000 + 1] << 48 | sp_dmem[phys - 0x04000000 + 2] << 40 |
               sp_dmem[phys - 0x04000000 + 3] << 32 | sp_dmem[phys - 0x04000000 + 4] << 24 | sp_dmem[phys - 0x04000000 + 5] << 16 |
               sp_dmem[phys - 0x04000000 + 6] << 8 | sp_dmem[phys - 0x04000000 + 7];
    }

    else if (phys >= 0x04001000 && phys <= 0x04001FFF)
    {
        return sp_imem[phys - 0x04001000] << 56 | sp_imem[phys - 0x04001000 + 1] << 48 | sp_imem[phys - 0x04001000 + 2] << 40 |
               sp_imem[phys - 0x04001000 + 3] << 32 | sp_imem[phys - 0x04001000 + 4] << 24 | sp_imem[phys - 0x04001000 + 5] << 16 |
               sp_imem[phys - 0x04001000 + 6] << 8 | sp_imem[phys - 0x04001000 + 7];
    }

    else if (phys >= 0x04040000 && phys <= 0x040FFFFF)
    {
        return sp_regs[phys - 0x04040000] << 56 | sp_regs[phys - 0x04040000 + 1] << 48 | sp_regs[phys - 0x04040000 + 2] << 40 |
               sp_regs[phys - 0x04040000 + 3] << 32 | sp_regs[phys - 0x04040000 + 4] << 24 | sp_regs[phys - 0x04040000 + 5] << 16 |
               sp_regs[phys - 0x04040000 + 6] << 8 | sp_regs[phys - 0x04040000 + 7];
    }

    else if (phys >= 0x04100000 && phys <= 0x041FFFFF)
    {
        return dp_commds[phys - 0x04100000] << 56 | dp_commds[phys - 0x04100000 + 1] << 48 | dp_commds[phys - 0x04100000 + 2] << 40 |
               dp_commds[phys - 0x04100000 + 3] << 32 | dp_commds[phys - 0x04100000 + 4] << 24 | dp_commds[phys - 0x04100000 + 5] << 16 |
               dp_commds[phys - 0x04100000 + 6] << 8 | dp_commds[phys - 0x04100000 + 7];
    }

    else if (phys >= 0x04300000 && phys <= 0x043FFFFF)
    {
        return mips_int[phys - 0x04300000] << 56 | mips_int[phys - 0x04300000 + 1] << 48 | mips_int[phys - 0x04300000 + 2] << 40 |
               mips_int[phys - 0x04300000 + 3] << 32 | mips_int[phys - 0x04300000 + 4] << 24 | mips_int[phys - 0x04300000 + 5] << 16 |
               mips_int[phys - 0x04300000 + 6] << 8 | mips_int[phys - 0x04300000 + 7];
    }

    else if (phys >= 0x04400000 && phys <= 0x044FFFFF)
    {
        return video_int[phys - 0x04400000] << 56 | video_int[phys - 0x04400000 + 1] << 48 | video_int[phys - 0x04400000 + 2] << 40 |
               video_int[phys - 0x04400000 + 3] << 32 | video_int[phys - 0x04400000 + 4] << 24 | video_int[phys - 0x04400000 + 5] << 16 |
               video_int[phys - 0x04400000 + 6] << 8 | video_int[phys - 0x04400000 + 7];
    }

    else if (phys >= 0x04500000 && phys <= 0x045FFFFF)
    {
        return audio_int[phys - 0x04500000] << 56 | audio_int[phys - 0x04500000 + 1] << 48 | audio_int[phys - 0x04500000 + 2] << 40 |
               audio_int[phys - 0x04500000 + 3] << 32 | audio_int[phys - 0x04500000 + 4] << 24 | audio_int[phys - 0x04500000 + 5] << 16 |
               audio_int[phys - 0x04500000 + 6] << 8 | audio_int[phys - 0x04500000 + 7];
    }

    else if (phys >= 0x04600000 && phys <= 0x046FFFFF)
    {
        return periph_int[phys - 0x04600000] << 56 | periph_int[phys - 0x04600000 + 1] << 48 | periph_int[phys - 0x04600000 + 2] << 40 |
               periph_int[phys - 0x04600000 + 3] << 32 | periph_int[phys - 0x04600000 + 4] << 24 | periph_int[phys - 0x04600000 + 5] << 16 |
               periph_int[phys - 0x04600000 + 6] << 8 | periph_int[phys - 0x04600000 + 7];
    }

    else if (phys >= 0x04700000 && phys <= 0x047FFFFF)
    {
        return rdram_int[phys - 0x04700000] << 56 | rdram_int[phys - 0x04700000 + 1] << 48 | rdram_int[phys - 0x04700000 + 2] << 40 |
               rdram_int[phys - 0x04700000 + 3] << 32 | rdram_int[phys - 0x04700000 + 4] << 24 | rdram_int[phys - 0x04700000 + 5] << 16 |
               rdram_int[phys - 0x04700000 + 6] << 8 | rdram_int[phys - 0x04700000 + 7];
    }

    else if (phys >= 0x04800000 && phys <= 0x048FFFFF)
    {
        return serial_int[phys - 0x04800000] << 56 | serial_int[phys - 0x04800000 + 1] << 48 | serial_int[phys - 0x04800000 + 2] << 40 |
               serial_int[phys - 0x04800000 + 3] << 32 | serial_int[phys - 0x04800000 + 4] << 24 | serial_int[phys - 0x04800000 + 5] << 16 |
               serial_int[phys - 0x04800000 + 6] << 8 | serial_int[phys - 0x04800000 + 7];
    }

    else if (phys >= 0x05000000 && phys <= 0x1FBFFFFF)
    {
        uint64_t x = ((uint64_t)(rom[phys - 0x05000000]) << 56) | ((uint64_t)(rom[phys - 0x05000000 + 1]) << 48) | ((uint64_t)(rom[phys - 0x05000000 + 2] << 40)) |
                     ((uint64_t)(rom[phys - 0x05000000 + 3]) << 32) | ((uint64_t)(rom[phys - 0x05000000 + 4]) << 24) | ((uint64_t)(rom[phys - 0x05000000 + 5]) << 16) |
                     ((uint64_t)(rom[phys - 0x05000000 + 6]) << 8) | (rom[phys - 0x05000000 + 7]);

        return x;
    }

    else if (phys >= 0x1FC00000 && phys <= 0x1FC007BF)
    {
        return pif_rom[phys - 0x1FC00000] << 56 | pif_rom[phys - 0x1FC00000 + 1] << 48 | pif_rom[phys - 0x1FC00000 + 2] << 40 |
               pif_rom[phys - 0x1FC00000 + 3] << 32 | pif_rom[phys - 0x1FC00000 + 4] << 24 | pif_rom[phys - 0x1FC00000 + 5] << 16 |
               pif_rom[phys - 0x1FC00000 + 6] << 8 | pif_rom[phys - 0x1FC00000 + 7];
    }

    else if (phys >= 0x1FC007C0 && phys <= 0x1FC007FF)
    {
        return pif_ram[phys - 0x1FC007FF] << 56 | pif_ram[phys - 0x1FC007FF + 1] << 48 | pif_ram[phys - 0x1FC007FF + 2] << 40 |
               pif_ram[phys - 0x1FC007FF + 3] << 32 | pif_ram[phys - 0x1FC007FF + 4] << 24 | pif_ram[phys - 0x1FC007FF + 5] << 16 |
               pif_ram[phys - 0x1FC007FF + 6] << 8 | pif_ram[phys - 0x1FC007FF + 7];
    }

    else if (phys >= 0x1FD00000 && phys <= 0x7FFFFFFF)
    {
        return rom[phys - 0x05000000] << 56 | rom[phys - 0x05000000 + 1] << 48 | rom[phys - 0x05000000 + 2] << 40 |
               rom[phys - 0x05000000 + 3] << 32 | rom[phys - 0x05000000 + 4] << 24 | rom[phys - 0x05000000 + 5] << 16 |
               rom[phys - 0x05000000 + 6] << 8 | rom[phys - 0x05000000 + 7];
    }
}

uint32_t MMU::read32(uint64_t addr)
{
    uint64_t phys = virt_to_phys(addr);
    if (phys >= 0x00000000 && phys <= 0x007FFFFF)
    {
        return rdram[phys] << 24 | rdram[phys + 1] << 16 |
               rdram[phys + 2] << 8 | rdram[phys + 3];
    }

    if (phys >= 0x03F00000 && phys <= 0x03FFFFFF)
    {
        return rdram_regs[phys - 0x03F00000] << 24 | rdram_regs[phys - 0x03F00000 + 1] << 16 |
               rdram_regs[phys - 0x03F00000 + 2] << 8 | rdram_regs[phys - 0x03F00000 + 3];
    }

    if (phys >= 0x04000000 && phys <= 0x04000FFF)
    {
        return (uint64_t)(sp_dmem[phys - 0x04000000]) << 24 | (uint64_t)(sp_dmem[phys - 0x04000000 + 1]) << 16 |
               (uint64_t)(sp_dmem[phys - 0x04000000 + 2]) << 8 | sp_dmem[phys - 0x04000000 + 3];
    }

    if (phys >= 0x04001000 && phys <= 0x04001FFF)
    {
        return sp_imem[phys - 0x04001000] << 24 | sp_imem[phys - 0x04001000 + 1] << 16 |
               sp_imem[phys - 0x04001000 + 2] << 8 | sp_imem[phys - 0x04001000 + 3];
    }

    if (phys >= 0x04040000 && phys <= 0x040FFFFF)
    {
        return sp_regs[phys - 0x04040000] << 24 | sp_regs[phys - 0x04040000 + 1] << 16 |
               sp_regs[phys - 0x04040000 + 2] << 8 | sp_regs[phys - 0x04040000 + 3];
    }

    if (phys >= 0x04100000 && phys <= 0x041FFFFF)
    {
        return dp_commds[phys - 0x04100000] << 24 | dp_commds[phys - 0x04100000 + 1] << 16 |
               dp_commds[phys - 0x04100000 + 2] << 8 | dp_commds[phys - 0x04100000 + 3];
    }

    if (phys >= 0x04300000 && phys <= 0x043FFFFF)
    {
        return mips_int[phys - 0x04300000] << 24 | mips_int[phys - 0x04300000 + 1] << 16 |
               mips_int[phys - 0x04300000 + 2] << 8 | mips_int[phys - 0x04300000 + 3];
    }

    if (phys >= 0x04400000 && phys <= 0x044FFFFF)
    {
        return video_int[phys - 0x04400000] << 24 | video_int[phys - 0x04400000 + 1] << 16 |
               video_int[phys - 0x04400000 + 2] << 8 | video_int[phys - 0x04400000 + 3];
    }

    if (phys >= 0x04500000 && phys <= 0x045FFFFF)
    {
        return audio_int[phys - 0x04500000] << 24 | audio_int[phys - 0x04500000 + 5] << 16 |
               audio_int[phys - 0x04500000 + 6] << 8 | audio_int[phys - 0x04500000 + 7];
    }

    if (phys >= 0x04600000 && phys <= 0x046FFFFF)
    {
        return periph_int[phys - 0x04600000] << 24 | periph_int[phys - 0x04600000 + 1] << 16 |
               periph_int[phys - 0x04600000 + 2] << 8 | periph_int[phys - 0x04600000 + 3];
    }

    if (phys >= 0x04700000 && phys <= 0x047FFFFF)
    {
        return rdram_int[phys - 0x04700000] << 24 | rdram_int[phys - 0x04700000 + 1] << 16 |
               rdram_int[phys - 0x04700000 + 2] << 8 | rdram_int[phys - 0x04700000 + 3];
    }

    if (phys >= 0x04800000 && phys <= 0x048FFFFF)
    {
        return serial_int[phys - 0x04800000] << 24 | serial_int[phys - 0x04800000 + 1] << 16 |
               serial_int[phys - 0x04800000 + 2] << 8 | serial_int[phys - 0x04800000 + 3];
    }

    if (phys >= 0x05000000 && phys <= 0x1FBFFFFF)
    {
        return rom[phys - 0x05000000] << 24 | rom[phys - 0x05000000 + 1] << 16 |
               rom[phys - 0x05000000 + 2] << 8 | rom[phys - 0x05000000 + 3];
    }

    if (phys >= 0x1FC00000 && phys <= 0x1FC007BF)
    {
        return pif_rom[phys - 0x1FC00000] << 24 | pif_rom[phys - 0x1FC00000 + 1] << 16 |
               pif_rom[phys - 0x1FC00000 + 2] << 8 | pif_rom[phys - 0x1FC00000 + 3];
    }

    if (phys >= 0x1FC007C0 && phys <= 0x1FC007FF)
    {
        return pif_ram[phys - 0x1FC007FF] << 24 | pif_ram[phys - 0x1FC007FF + 1] << 16 |
               pif_ram[phys - 0x1FC007FF + 2] << 8 | pif_ram[phys - 0x1FC007FF + 3];
    }

    if (phys >= 0x1FD00000 && phys <= 0x7FFFFFFF)
    {
        return rom[phys - 0x05000000] << 24 | rom[phys - 0x05000000 + 1] << 16 |
               rom[phys - 0x05000000 + 2] << 8 | rom[phys - 0x05000000 + 3];
    }
}

uint16_t MMU::read16(uint64_t addr)
{
    uint64_t phys = virt_to_phys(addr);
    if (phys >= 0x00000000 && phys <= 0x007FFFFF)
    {
        return rdram[phys] << 8 | rdram[phys + 1];
    }

    if (phys >= 0x03F00000 && phys <= 0x3FFFFFFF)
    {
        return rdram_regs[phys - 0x03F00000] << 8 | rdram_regs[phys - 0x03F00000 + 1];
    }

    if (phys >= 0x04000000 && phys <= 0x04000FFF)
    {
        return sp_dmem[phys - 0x04000000] << 8 | sp_dmem[phys - 0x04000000 + 1];
    }

    if (phys >= 0x04001000 && phys <= 0x04001FFF)
    {
        return sp_imem[phys - 0x04001000] << 8 | sp_imem[phys - 0x04001000 + 1];
    }

    if (phys >= 0x04040000 && phys <= 0x040FFFFF)
    {
        return sp_regs[phys - 0x04040000] << 8 | sp_regs[phys - 0x04040000 + 1];
    }

    if (phys >= 0x04100000 && phys <= 0x041FFFFF)
    {
        return dp_commds[phys - 0x04100000] << 8 | dp_commds[phys - 0x04100000 + 1];
    }

    if (phys >= 0x04300000 && phys <= 0x043FFFFF)
    {
        return mips_int[phys - 0x04300000] << 8 | mips_int[phys - 0x04300000 + 1];
    }

    if (phys >= 0x04400000 && phys <= 0x044FFFFF)
    {
        return video_int[phys - 0x04400000] << 8 | video_int[phys - 0x04400000 + 1];
    }

    if (phys >= 0x04500000 && phys <= 0x045FFFFF)
    {
        return audio_int[phys - 0x04500000 + 6] << 8 | audio_int[phys - 0x04500000 + 7];
    }

    if (phys >= 0x04600000 && phys <= 0x046FFFFF)
    {
        return periph_int[phys - 0x04600000] << 8 | periph_int[phys - 0x04600000 + 1];
    }

    if (phys >= 0x04700000 && phys <= 0x047FFFFF)
    {
        return rdram_int[phys - 0x04700000] << 8 | rdram_int[phys - 0x04700000 + 1];
    }

    if (phys >= 0x04800000 && phys <= 0x048FFFFF)
    {
        return serial_int[phys - 0x04800000] << 8 | serial_int[phys - 0x04800000 + 1];
    }

    if (phys >= 0x05000000 && phys <= 0x1FBFFFFF)
    {
        return rom[phys - 0x05000000] << 8 | rom[phys - 0x05000000 + 1];
    }

    if (phys >= 0x1FC00000 && phys <= 0x1FC007BF)
    {
        return pif_rom[phys - 0x1FC00000] << 8 | pif_rom[phys - 0x1FC00000 + 1];
    }

    if (phys >= 0x1FC007C0 && phys <= 0x1FC007FF)
    {
        return pif_ram[phys - 0x1FC007FF] << 8 | pif_ram[phys - 0x1FC007FF + 1];
    }

    if (phys >= 0x1FD00000 && phys <= 0x7FFFFFFF)
    {
        return rom[phys - 0x05000000] << 8 | rom[phys - 0x05000000 + 1];
    }
}

uint8_t MMU::read8(uint64_t addr)
{
    uint64_t phys = virt_to_phys(addr);
    if (phys >= 0x00000000 && phys <= 0x007FFFFF)
    {
        return rdram[phys];
    }

    if (phys >= 0x03F00000 && phys <= 0x3FFFFFFF)
    {
        return rdram_regs[phys - 0x03F00000];
    }

    if (phys >= 0x04000000 && phys <= 0x04000FFF)
    {
        return sp_dmem[phys - 0x04000000];
    }

    if (phys >= 0x04001000 && phys <= 0x04001FFF)
    {
        return sp_imem[phys - 0x04001000];
    }

    if (phys >= 0x04040000 && phys <= 0x040FFFFF)
    {
        return sp_regs[phys - 0x04040000];
    }

    if (phys >= 0x04100000 && phys <= 0x041FFFFF)
    {
        return dp_commds[phys - 0x04100000];
    }

    if (phys >= 0x04300000 && phys <= 0x043FFFFF)
    {
        return mips_int[phys - 0x04300000];
    }

    if (phys >= 0x04400000 && phys <= 0x044FFFFF)
    {
        return video_int[phys - 0x04400000];
    }

    if (phys >= 0x04500000 && phys <= 0x045FFFFF)
    {
        return audio_int[phys - 0x04500000 + 7];
    }

    if (phys >= 0x04600000 && phys <= 0x046FFFFF)
    {
        return periph_int[phys - 0x04600000];
    }

    if (phys >= 0x04700000 && phys <= 0x047FFFFF)
    {
        return rdram_int[phys - 0x04700000];
    }

    if (phys >= 0x04800000 && phys <= 0x048FFFFF)
    {
        return serial_int[phys - 0x04800000];
    }

    if (phys >= 0x05000000 && phys <= 0x1FBFFFFF)
    {
        return rom[phys - 0x05000000];
    }

    if (phys >= 0x1FC00000 && phys <= 0x1FC007BF)
    {
        return pif_rom[phys - 0x1FC00000];
    }

    if (phys >= 0x1FC007C0 && phys <= 0x1FC007FF)
    {
        return pif_ram[phys - 0x1FC007FF];
    }

    if (phys >= 0x1FD00000 && phys <= 0x7FFFFFFF)
    {
        return rom[phys - 0x05000000];
    }
}

void MMU::write64(uint64_t addr, uint64_t value)
{
    uint8_t nibble1, nibble2, nibble3, nibble4, nibble5, nibble6, nibble7, nibble8;
    nibble8 = (value >> 56) & 0b1111'1111;
    nibble7 = (value >> 48) & 0b1111'1111;
    nibble6 = (value >> 40) & 0b1111'1111;
    nibble5 = (value >> 32) & 0b1111'1111;
    nibble4 = (value >> 24) & 0b1111'1111;
    nibble3 = (value >> 16) & 0b1111'1111;
    nibble2 = (value >> 8) & 0b1111'1111;
    nibble1 = value & 0b1111'1111;

    uint64_t phys = virt_to_phys(addr);
    if (phys >= 0x00000000 && phys <= 0x007FFFFF)
    {
        rdram[phys] = nibble8;
        rdram[phys + 1] = nibble7;
        rdram[phys + 2] = nibble6;
        rdram[phys + 3] = nibble5;
        rdram[phys + 4] = nibble4;
        rdram[phys + 5] = nibble3;
        rdram[phys + 6] = nibble2;
        rdram[phys + 7] = nibble1;
    }

    if (phys >= 0x03F00000 && phys <= 0x03FFFFFF)
    {
        rdram_regs[phys - 0x03F00000] = nibble8;
        rdram_regs[phys - 0x03F00000 + 1] = nibble7;
        rdram_regs[phys - 0x03F00000 + 2] = nibble6;
        rdram_regs[phys - 0x03F00000 + 3] = nibble5;
        rdram_regs[phys - 0x03F00000 + 4] = nibble4;
        rdram_regs[phys - 0x03F00000 + 5] = nibble3;
        rdram_regs[phys - 0x03F00000 + 6] = nibble2;
        rdram_regs[phys - 0x03F00000 + 7] = nibble1;
    }

    if (phys >= 0x04000000 && phys <= 0x04000FFF)
    {
        sp_dmem[phys - 0x04000000] = nibble8;
        sp_dmem[phys - 0x04000000 + 1] = nibble7;
        sp_dmem[phys - 0x04000000 + 2] = nibble6;
        sp_dmem[phys - 0x04000000 + 3] = nibble5;
        sp_dmem[phys - 0x04000000 + 4] = nibble4;
        sp_dmem[phys - 0x04000000 + 5] = nibble3;
        sp_dmem[phys - 0x04000000 + 6] = nibble2;
        sp_dmem[phys - 0x04000000 + 7] = nibble1;
    }

    if (phys >= 0x04001000 && phys <= 0x04001FFF)
    {
        sp_imem[phys - 0x04001000] = nibble8;
        sp_imem[phys - 0x04001000 + 1] = nibble7;
        sp_imem[phys - 0x04001000 + 2] = nibble6;
        sp_imem[phys - 0x04001000 + 3] = nibble5;
        sp_imem[phys - 0x04001000 + 4] = nibble4;
        sp_imem[phys - 0x04001000 + 5] = nibble3;
        sp_imem[phys - 0x04001000 + 6] = nibble2;
        sp_imem[phys - 0x04001000 + 7] = nibble1;
    }

    if (phys >= 0x04040000 && phys <= 0x040FFFFF)
    {
        sp_regs[phys - 0x04040000] = nibble8;
        sp_regs[phys - 0x04040000 + 1] = nibble7;
        sp_regs[phys - 0x04040000 + 2] = nibble6;
        sp_regs[phys - 0x04040000 + 3] = nibble5;
        sp_regs[phys - 0x04040000 + 4] = nibble4;
        sp_regs[phys - 0x04040000 + 5] = nibble3;
        sp_regs[phys - 0x04040000 + 6] = nibble2;
        sp_regs[phys - 0x04040000 + 7] = nibble1;
    }

    if (phys >= 0x04100000 && phys <= 0x041FFFFF)
    {
        dp_commds[phys - 0x04100000] = nibble8;
        dp_commds[phys - 0x04100000 + 1] = nibble7;
        dp_commds[phys - 0x04100000 + 2] = nibble6;
        dp_commds[phys - 0x04100000 + 3] = nibble5;
        dp_commds[phys - 0x04100000 + 4] = nibble4;
        dp_commds[phys - 0x04100000 + 5] = nibble3;
        dp_commds[phys - 0x04100000 + 6] = nibble2;
        dp_commds[phys - 0x04100000 + 7] = nibble1;
    }

    if (phys >= 0x04300000 && phys <= 0x043FFFFF)
    {
        mips_int[phys - 0x04300000] = nibble8;
        mips_int[phys - 0x04300000 + 1] = nibble7;
        mips_int[phys - 0x04300000 + 2] = nibble6;
        mips_int[phys - 0x04300000 + 3] = nibble5;
        mips_int[phys - 0x04300000 + 4] = nibble4;
        mips_int[phys - 0x04300000 + 5] = nibble3;
        mips_int[phys - 0x04300000 + 6] = nibble2;
        mips_int[phys - 0x04300000 + 7] = nibble1;
    }

    if (phys >= 0x04400000 && phys <= 0x044FFFFF)
    {
        video_int[phys - 0x04400000] = nibble8;
        video_int[phys - 0x04400000 + 1] = nibble7;
        video_int[phys - 0x04400000 + 2] = nibble6;
        video_int[phys - 0x04400000 + 3] = nibble5;
        video_int[phys - 0x04400000 + 4] = nibble4;
        video_int[phys - 0x04400000 + 5] = nibble3;
        video_int[phys - 0x04400000 + 6] = nibble2;
        video_int[phys - 0x04400000 + 7] = nibble1;
    }

    if (phys >= 0x04500000 && phys <= 0x045FFFFF)
    {
        audio_int[phys - 0x04500000] = nibble8;
        audio_int[phys - 0x04500000 + 1] = nibble7;
        audio_int[phys - 0x04500000 + 2] = nibble6;
        audio_int[phys - 0x04500000 + 3] = nibble5;
        audio_int[phys - 0x04500000 + 4] = nibble4;
        audio_int[phys - 0x04500000 + 5] = nibble3;
        audio_int[phys - 0x04500000 + 6] = nibble2;
        audio_int[phys - 0x04500000 + 7] = nibble1;
    }

    if (phys >= 0x04600000 && phys <= 0x046FFFFF)
    {
        periph_int[phys - 0x04600000] = nibble8;
        periph_int[phys - 0x04600000 + 1] = nibble7;
        periph_int[phys - 0x04600000 + 2] = nibble6;
        periph_int[phys - 0x04600000 + 3] = nibble5;
        periph_int[phys - 0x04600000 + 4] = nibble4;
        periph_int[phys - 0x04600000 + 5] = nibble3;
        periph_int[phys - 0x04600000 + 6] = nibble2;
        periph_int[phys - 0x04600000 + 7] = nibble1;
    }

    if (phys >= 0x04700000 && phys <= 0x047FFFFF)
    {
        rdram_int[phys - 0x04700000] = nibble8;
        rdram_int[phys - 0x04700000 + 1] = nibble7;
        rdram_int[phys - 0x04700000 + 2] = nibble6;
        rdram_int[phys - 0x04700000 + 3] = nibble5;
        rdram_int[phys - 0x04700000 + 4] = nibble4;
        rdram_int[phys - 0x04700000 + 5] = nibble3;
        rdram_int[phys - 0x04700000 + 6] = nibble2;
        rdram_int[phys - 0x04700000 + 7] = nibble1;
    }

    if (phys >= 0x04800000 && phys <= 0x048FFFFF)
    {
        serial_int[phys - 0x04800000] = nibble8;
        serial_int[phys - 0x04800000 + 1] = nibble7;
        serial_int[phys - 0x04800000 + 2] = nibble6;
        serial_int[phys - 0x04800000 + 3] = nibble5;
        serial_int[phys - 0x04800000 + 4] = nibble4;
        serial_int[phys - 0x04800000 + 5] = nibble3;
        serial_int[phys - 0x04800000 + 6] = nibble2;
        serial_int[phys - 0x04800000 + 7] = nibble1;
    }

    if (phys >= 0x1FC007C0 && phys <= 0x1FC007FF)
    {
        pif_ram[phys - 0x1FC007C0] = nibble8;
        pif_ram[phys - 0x1FC007C0 + 1] = nibble7;
        pif_ram[phys - 0x1FC007C0 + 2] = nibble6;
        pif_ram[phys - 0x1FC007C0 + 3] = nibble5;
        pif_ram[phys - 0x1FC007C0 + 4] = nibble4;
        pif_ram[phys - 0x1FC007C0 + 5] = nibble3;
        pif_ram[phys - 0x1FC007C0 + 6] = nibble2;
        pif_ram[phys - 0x1FC007C0 + 7] = nibble1;
    }
}

void MMU::write32(uint64_t addr, uint32_t value)
{
    uint8_t nibble1, nibble2, nibble3, nibble4;
    nibble4 = (value >> 24) & 0b1111'1111;
    nibble3 = (value >> 16) & 0b1111'1111;
    nibble2 = (value >> 8) & 0b1111'1111;
    nibble1 = value & 0b1111'1111;

    uint64_t phys = virt_to_phys(addr);
    if (phys >= 0x00000000 && phys <= 0x007FFFFF)
    {
        rdram_regs[phys - 0x03F00000] = nibble4;
        rdram_regs[phys - 0x03F00000 + 1] = nibble3;
        rdram_regs[phys - 0x03F00000 + 2] = nibble2;
        rdram_regs[phys - 0x03F00000 + 3] = nibble1;
    }

    if (phys >= 0x04000000 && phys <= 0x04000FFF)
    {
        sp_dmem[phys - 0x04000000] = nibble4;
        sp_dmem[phys - 0x04000000 + 1] = nibble3;
        sp_dmem[phys - 0x04000000 + 2] = nibble2;
        sp_dmem[phys - 0x04000000 + 3] = nibble1;
    }

    if (phys >= 0x04001000 && phys <= 0x04001FFF)
    {
        sp_imem[phys - 0x04001000] = nibble4;
        sp_imem[phys - 0x04001000 + 1] = nibble3;
        sp_imem[phys - 0x04001000 + 2] = nibble2;
        sp_imem[phys - 0x04001000 + 3] = nibble1;
    }

    if (phys >= 0x04040000 && phys <= 0x040FFFFF)
    {
        sp_regs[phys - 0x04040000] = nibble4;
        sp_regs[phys - 0x04040000 + 1] = nibble3;
        sp_regs[phys - 0x04040000 + 2] = nibble2;
        sp_regs[phys - 0x04040000 + 3] = nibble1;
    }

    if (phys >= 0x04100000 && phys <= 0x041FFFFF)
    {
        dp_commds[phys - 0x04100000] = nibble4;
        dp_commds[phys - 0x04100000 + 1] = nibble3;
        dp_commds[phys - 0x04100000 + 2] = nibble2;
        dp_commds[phys - 0x04100000 + 3] = nibble1;
    }

    if (phys >= 0x04300000 && phys <= 0x043FFFFF)
    {
        mips_int[phys - 0x04300000] = nibble4;
        mips_int[phys - 0x04300000 + 1] = nibble3;
        mips_int[phys - 0x04300000 + 2] = nibble2;
        mips_int[phys - 0x04300000 + 3] = nibble1;
    }

    if (phys >= 0x04400000 && phys <= 0x044FFFFF)
    {
        video_int[phys - 0x04400000] = nibble4;
        video_int[phys - 0x04400000 + 1] = nibble3;
        video_int[phys - 0x04400000 + 2] = nibble2;
        video_int[phys - 0x04400000 + 3] = nibble1;
    }

    if (phys >= 0x04500000 && phys <= 0x045FFFFF)
    {
        audio_int[phys - 0x04500000] = nibble4;
        audio_int[phys - 0x04500000 + 1] = nibble3;
        audio_int[phys - 0x04500000 + 2] = nibble2;
        audio_int[phys - 0x04500000 + 3] = nibble1;
    }

    if (phys >= 0x04600000 && phys <= 0x046FFFFF)
    {
        periph_int[phys - 0x04600000] = nibble4;
        periph_int[phys - 0x04600000 + 1] = nibble3;
        periph_int[phys - 0x04600000 + 2] = nibble2;
        periph_int[phys - 0x04600000 + 3] = nibble1;
    }

    if (phys >= 0x04700000 && phys <= 0x047FFFFF)
    {
        rdram_int[phys - 0x04700000] = nibble4;
        rdram_int[phys - 0x04700000 + 1] = nibble3;
        rdram_int[phys - 0x04700000 + 2] = nibble2;
        rdram_int[phys - 0x04700000 + 3] = nibble1;
    }

    if (phys >= 0x04800000 && phys <= 0x048FFFFF)
    {
        serial_int[phys - 0x04800000] = nibble4;
        serial_int[phys - 0x04800000 + 1] = nibble3;
        serial_int[phys - 0x04800000 + 2] = nibble2;
        serial_int[phys - 0x04800000 + 3] = nibble1;
    }

    if (phys >= 0x1FC007C0 && phys <= 0x1FC007FF)
    {
        pif_ram[phys - 0x1FC007C0] = nibble4;
        pif_ram[phys - 0x1FC007C0 + 1] = nibble3;
        pif_ram[phys - 0x1FC007C0 + 2] = nibble2;
        pif_ram[phys - 0x1FC007C0 + 3] = nibble1;
    }
}

void MMU::write16(uint64_t addr, uint16_t value)
{
    uint8_t nibble1, nibble2;
    nibble2 = (value >> 8) & 0b1111'1111;
    nibble1 = value & 0b1111'1111;

    uint64_t phys = virt_to_phys(addr);
    if (phys >= 0x00000000 && phys <= 0x007FFFFF)
    {
        rdram_regs[phys - 0x03F00000] = nibble2;
        rdram_regs[phys - 0x03F00000 + 1] = nibble1;
    }

    if (phys >= 0x04000000 && phys <= 0x04000FFF)
    {
        sp_dmem[phys - 0x04000000] = nibble2;
        sp_dmem[phys - 0x04000000 + 1] = nibble1;
    }

    if (phys >= 0x04001000 && phys <= 0x04001FFF)
    {
        sp_imem[phys - 0x04001000] = nibble2;
        sp_imem[phys - 0x04001000 + 1] = nibble1;
    }

    if (phys >= 0x04040000 && phys <= 0x040FFFFF)
    {
        sp_regs[phys - 0x04040000] = nibble2;
        sp_regs[phys - 0x04040000 + 1] = nibble1;
    }

    if (phys >= 0x04100000 && phys <= 0x041FFFFF)
    {
        dp_commds[phys - 0x04100000] = nibble2;
        dp_commds[phys - 0x04100000 + 1] = nibble1;
    }

    if (phys >= 0x04300000 && phys <= 0x043FFFFF)
    {
        mips_int[phys - 0x04300000] = nibble2;
        mips_int[phys - 0x04300000 + 1] = nibble1;
    }

    if (phys >= 0x04400000 && phys <= 0x044FFFFF)
    {
        video_int[phys - 0x04400000] = nibble2;
        video_int[phys - 0x04400000 + 1] = nibble1;
    }

    if (phys >= 0x04500000 && phys <= 0x045FFFFF)
    {
        audio_int[phys - 0x04500000] = nibble2;
        audio_int[phys - 0x04500000 + 1] = nibble1;
    }

    if (phys >= 0x04600000 && phys <= 0x046FFFFF)
    {
        periph_int[phys - 0x04600000] = nibble2;
        periph_int[phys - 0x04600000 + 1] = nibble1;
    }

    if (phys >= 0x04700000 && phys <= 0x047FFFFF)
    {
        rdram_int[phys - 0x04700000] = nibble2;
        rdram_int[phys - 0x04700000 + 1] = nibble1;
    }

    if (phys >= 0x04800000 && phys <= 0x048FFFFF)
    {
        serial_int[phys - 0x04800000] = nibble2;
        serial_int[phys - 0x04800000 + 1] = nibble1;
    }

    if (phys >= 0x1FC007C0 && phys <= 0x1FC007FF)
    {
        pif_ram[phys - 0x1FC007C0] = nibble2;
        pif_ram[phys - 0x1FC007C0 + 1] = nibble1;
    }
}

void MMU::write8(uint64_t addr, uint8_t value)
{
    uint8_t nibble1;
    nibble1 = value & 0b1111'1111;

    uint64_t phys = virt_to_phys(addr);
    if (phys >= 0x00000000 && phys <= 0x007FFFFF)
    {
        rdram_regs[phys - 0x03F00000 + 1] = nibble1;
    }

    if (phys >= 0x04000000 && phys <= 0x04000FFF)
    {
        sp_dmem[phys - 0x04000000 + 1] = nibble1;
    }

    if (phys >= 0x04001000 && phys <= 0x04001FFF)
    {
        sp_imem[phys - 0x04001000 + 1] = nibble1;
    }

    if (phys >= 0x04040000 && phys <= 0x040FFFFF)
    {
        sp_regs[phys - 0x04040000 + 1] = nibble1;
    }

    if (phys >= 0x04100000 && phys <= 0x041FFFFF)
    {
        dp_commds[phys - 0x04100000 + 1] = nibble1;
    }

    if (phys >= 0x04300000 && phys <= 0x043FFFFF)
    {
        mips_int[phys - 0x04300000 + 1] = nibble1;
    }

    if (phys >= 0x04400000 && phys <= 0x044FFFFF)
    {
        video_int[phys - 0x04400000 + 1] = nibble1;
    }

    if (phys >= 0x04500000 && phys <= 0x045FFFFF)
    {
        audio_int[phys - 0x04500000 + 1] = nibble1;
    }

    if (phys >= 0x04600000 && phys <= 0x046FFFFF)
    {
        periph_int[phys - 0x04600000 + 1] = nibble1;
    }

    if (phys >= 0x04700000 && phys <= 0x047FFFFF)
    {
        rdram_int[phys - 0x04700000 + 1] = nibble1;
    }

    if (phys >= 0x04800000 && phys <= 0x048FFFFF)
    {
        serial_int[phys - 0x04800000 + 1] = nibble1;
    }

    if (phys >= 0x1FC007C0 && phys <= 0x1FC007FF)
    {
        pif_ram[phys - 0x1FC007C0 + 1] = nibble1;
    }
}

void MMU::load_rom(std::string path)
{
    rom.resize(268'435'456);

    FILE *file = fopen(path.c_str(), "rb");
    int pos = 0;

    while (fread(&rom[pos + 0x0B000000], 1, 1, file))
    {
        //std::cout << std::hex << (int)Memory.rom[pos] << " ";
        pos++;
    }

    // std::fstream fin(path.c_str(), std::ios::binary);
    // uint8_t x;
    // int k = 0;
    // while (fin >> x)
    // {
    //     rom[0x05000000 + k++] = x;
    // }
}

MMU::MMU()
{
    load_rom("./roms/basic.z64");
}