#include "obj/component.h"

namespace oglopp {
	/**
	 * @brief A component for a single 32 bit number. Used for particle system IDs, or whatever else. 
	 */
	struct CNum : public Component<CNum> {
		static constexpr uint32_t parts = HLGL_VEC_COMPONENTS;
		static constexpr DataType type = DataType::UINT32;

		uint64_t num;

		CNum(uint32_t newNum) : num(newNum) {}
	};
}
