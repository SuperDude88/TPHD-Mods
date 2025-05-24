#include <cstdint>

#include <vector>

#include <coreinit/title.h>
#include <coreinit/memorymap.h>
#include <coreinit/cache.h>

#include <wups.h>
#include <kernel/kernel.h>

WUPS_PLUGIN_NAME("TPHD Aesthetic Armor");
WUPS_PLUGIN_DESCRIPTION("Make the magic armor in Twilight Princess HD a double defense upgrade, without the rupee drain");
WUPS_PLUGIN_VERSION("v1.0");
WUPS_PLUGIN_AUTHOR("SuperDude88");
WUPS_PLUGIN_LICENSE("GPLv3");

static constexpr uint64_t TPHD_TID_JPN = 0x000500001019C800L;
static constexpr uint64_t TPHD_TID_USA = 0x000500001019E500L;
static constexpr uint64_t TPHD_TID_EUR = 0x000500001019E600L;

class InstructionPatch {
private:
    enum struct Status {
        UNPATCHED,
        PATCHED
    };

    using Instruction_t = uint32_t;

    Status status;
    const uint32_t vaddr;
    const Instruction_t replacement;
    Instruction_t original;

    void writeInstruction(const uint32_t& vaddr, const Instruction_t& instr) {
        KernelCopyData(OSEffectiveToPhysical(vaddr), OSEffectiveToPhysical((uint32_t)&instr), sizeof(Instruction_t));
        ICInvalidateRange((void*)vaddr, sizeof(Instruction_t));
        DCFlushRange((void*)vaddr, sizeof(Instruction_t));
    }

public:
    InstructionPatch(const uint32_t& vaddr_, const Instruction_t& instruction_) :
        status(Status::UNPATCHED),
        vaddr(vaddr_),
        replacement(instruction_)
    {
        KernelCopyData(OSEffectiveToPhysical((uint32_t)&original), OSEffectiveToPhysical(vaddr), sizeof(Instruction_t));
        DCFlushRange(&original, sizeof(Instruction_t));
    }

    void apply() {
        if(status == Status::PATCHED) {
            return;
        }

        writeInstruction(vaddr, replacement);
        status = Status::PATCHED;
    }

    void revert() {
        if(status == Status::UNPATCHED) {
            return;
        }

        writeInstruction(vaddr, original);
        status = Status::UNPATCHED;
    }
};

static std::vector<InstructionPatch> appliedPatches = {};

ON_APPLICATION_START() {
    appliedPatches.clear();

    const uint64_t tid = OSGetTitleID();
    
    switch(tid) {
        case TPHD_TID_JPN:
        case TPHD_TID_USA:
        case TPHD_TID_EUR:
            // Seems like all versions have basically identical code (JPN v64/82, USA v64/81, EUR v81)
            // JPN has a couple of new bytes somewhere but it doesn't seem to affect any of these addresses
            appliedPatches.emplace_back(0x0201A224, 0x38000001);
            appliedPatches.emplace_back(0x02087614, 0x4800006C);
            appliedPatches.emplace_back(0x02055FDC, 0x60000000);
            appliedPatches.emplace_back(0x020875E0, 0x60000000);
            appliedPatches.emplace_back(0x02056538, 0x60000000);
            appliedPatches.emplace_back(0x0201A3D0, 0x577BF87E);
            appliedPatches.emplace_back(0x0201A3D4, 0x48000028);
            break;
    }

    for(auto& patch : appliedPatches) {
        patch.apply();
    }
}

DEINITIALIZE_PLUGIN() {
    for(auto& patch : appliedPatches) {
        patch.revert();
    }
}
