#include "concept.h"
#include "synchronize.h"

namespace dark {

template <typename _Tp, typename _Vp>
inline void connect(_Tp &lhs, _Vp &&rhs);

namespace details {

	template <std::size_t _Size, std::size_t _This = 0>
	inline void connect_tuple(auto &lhs, auto &&rhs) {
		if constexpr (_This < _Size) {
			connect(std::get<_This>(lhs), std::get<_This>(rhs));
			connect_tuple<_Size, _This + 1>(lhs, rhs);
		}
	}

} // namespace details

/**
 * @brief Connects two objects.
 * Note that both should have the same structure.
 */
template<typename _Tp, typename _Vp>
inline void connect(_Tp &lhs, _Vp &&rhs) {
	using _Up = std::remove_cvref_t <_Vp>;
	if constexpr (!std::is_reference_v <_Vp>) {
		static_assert(std::is_reference_v <_Vp>,
			"RHS must be a reference, but not a right-value");
	}
	else if constexpr (std::is_assignable_v <_Tp, _Up>) {
		lhs = rhs;
	}
	else if constexpr (concepts::is_std_array_v <_Tp>) {
		static_assert(concepts::is_std_array_v <_Up>,
			"Both types must be std::array.");
		static_assert(
			concepts::array_size_v<_Tp>
		 == concepts::array_size_v<_Up>, "Size mismatch.");
		for (std::size_t i = 0; i < lhs.size(); ++i) {
			connect(lhs[i], rhs[i]);
		}
	}
	else if constexpr (std::is_aggregate_v <_Tp>) {
		static_assert(std::is_aggregate_v<_Up>,
			"Both types must be aggregate.");

		auto lhs_tuple = reflect::tuplify(lhs);
		auto rhs_tuple = reflect::tuplify(rhs);

		static_assert(
			std::tuple_size_v<decltype(lhs_tuple)>
		 == std::tuple_size_v<decltype(rhs_tuple)>, "Size mismatch.");
		constexpr auto kMembers = std::tuple_size_v<decltype(lhs_tuple)>;

		details::connect_tuple<kMembers>(lhs_tuple, rhs_tuple);
	}
	else {
		static_assert(sizeof(_Tp) == 0, "Unsupported types.");
	}
}

} // namespace dark
