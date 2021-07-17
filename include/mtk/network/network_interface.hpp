#ifndef MTK_NETWORK_NETWORK_INTERFACE_HPP
#define MTK_NETWORK_NETWORK_INTERFACE_HPP

#include <mtk/core/array.hpp>
#include <mtk/core/flag_operators.hpp>
#include <mtk/core/span.hpp>
#include <mtk/core/types.hpp>
#include <mtk/core/zstring_view.hpp>
#include <mtk/network/inet_address.hpp>

#include <string>

namespace mtk {

enum class network_interface_flag : uint32_t
{
	none =				0,
	up =				(1 << 0),
	running =			(1 << 1),
	loopback =			(1 << 2),
	broadcast =			(1 << 3),
	multicast =			(1 << 4),
	point_to_point =	(1 << 5)
};
MTK_DEFINE_FLAG_OPERATORS(network_interface_flag)



class network_interface
{
public:
	network_interface() noexcept :
		m_name(),
		m_index(0),
		m_flags(network_interface_flag::none),
		m_addresses()
	{ }

	zstring_view
	name() const noexcept
	{
		return m_name;
	}

	size_t
	index() const noexcept
	{
		return m_index;
	}

	network_interface_flag
	flags() const noexcept
	{
		return m_flags;
	}

	span<const inet_address>
	addresses() const noexcept
	{
		return m_addresses;
	}

private:
	friend array<network_interface> all_network_interfaces(bool*) noexcept;

	std::string m_name;
	size_t m_index;
	network_interface_flag m_flags;
	array<inet_address> m_addresses;
};



array<network_interface>
all_network_interfaces(bool* error = nullptr) noexcept;

} // namespace mtk

#endif
