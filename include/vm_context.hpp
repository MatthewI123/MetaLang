#pragma once

#include "common.hpp"
#include "type_list.hpp"
#include "value_list.hpp"


template<typename Registers, typename Memory, typename Interrupts, word_t ProgramCounter>
class vm_context
{
public:
	using registers = Registers;
	using memory = Memory;
	using interrupts = Interrupts;
	static constexpr word_t program_counter = ProgramCounter;

	/**
	 * \brief Reads the value stored in the given register.
	 * \tparam Register the register index, between 0 and 16 exclusive.
	 */
	template<word_t Register>
	static inline constexpr word_t read_register = Registers::template get<Register>;

	/**
	 * \brief Reads the value stored in memory at the given address.
	 * \tparam Address the address, between 0 and MEMORY exclusive.
	 */
	template<word_t Address>
	static inline constexpr word_t read_memory = Memory::template get<Address>;

	/**
	 * \brief Writes a value to the given register.
	 * \tparam Register the register index, between 0 and 16 exclusive.
	 * \tparam Value the value to write.
	 */
	template<word_t Register, word_t Value>
	using write_register = vm_context<typename Registers::template set<Register, Value>,
		Memory, Interrupts, ProgramCounter>;

	/**
	 * \brief Writes a value to memory at the given address.
	 * \tparam Address the address, between 0 and MEMORY exclusive.
	 * \tparam Value the value to write.
	 */
	template<word_t Address, word_t Value>
	using write_memory = vm_context<registers,
		typename Memory::template set<Address, Value>, Interrupts, ProgramCounter>;

	/**
	 * \brief Registers an interrupt handler to the given interrupt.
	 * \tparam Interrupt the interrupt number, between 0 and INTERRUPTS exclusive.
	 * \tparam Handler the handler.
	 */
	template<word_t Interrupt, typename Handler>
	using register_interrupt = vm_context<Registers, Memory,
		typename Interrupts::template set<Interrupt, Handler>, ProgramCounter>;

	/**
	 * \brief Decodes the next instruction.
	 */
	// using decode = decode_instruction<Memory::template get<ProgramCounter>>;
};

using make_vm_context_t = vm_context<make_value_list_t<0, 16>, make_value_list_t<0, MEMORY>,
	make_type_list_t<trivial_interrupt, INTERRUPTS>, 0>;
