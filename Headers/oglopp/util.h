#ifndef OGLOPP_UTIL_H
#define OGLOPP_UTIL_H

namespace oglopp {
	/**
	 * @brief Cast A* to type B by first casting to void*
	 * @description safep_cast<B.type>(A.ptr);
	 * @param[in] ptr	The input pointer to cast
	 * @return B		The resulting pointer
	 */
	template <typename B, typename A>
	inline constexpr B safep_cast(A* ptr) {
		return static_cast<B>(static_cast<void*>(ptr));
	}

	/**
	 * @brief Cast A const* to type B by first casting to void const*
	 * @description safepconst_cast<B.type>(A.ptr);
	 * @param[in] ptr	The input pointer to cast
	 * @return B		The resulting pointer
	 */
	template <typename B, typename A>
	inline constexpr B safepconst_cast(const A* ptr) {
		return static_cast<B>(static_cast<const void*>(ptr));
	}
}

#endif 
