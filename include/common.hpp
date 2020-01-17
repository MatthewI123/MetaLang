#pragma once

#include <cstddef>
#include <cstdint>

using word_t = std::uint16_t; ///> the virtual machine's word type

constexpr std::size_t MEMORY = 16; ///> memory allocated to VM in words (min: 16)
constexpr std::size_t INTERRUPTS = 8; ///> number of interrupts (min: 8)

/// Holds the VM's state.
template<typename Registers, typename Memory, typename Interrupts, word_t ProgramCounter>
class vm_context;

/// The VM state once it's been halted.
template<typename Context>
class vm_context_halted : Context
{
private:
	struct VM_CONTEXT_HALTED { };

public:
	using next = VM_CONTEXT_HALTED;
};

/// Default interrupt (does nothing).
struct trivial_interrupt
{
	template<typename Context>
	struct handler
	{
		using execute = Context;
	};
};
