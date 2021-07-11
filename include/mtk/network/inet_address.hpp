#ifndef MTK_NETWORK_INET_ADDRESS_HPP
#define MTK_NETWORK_INET_ADDRESS_HPP

#include <mtk/core/assert.hpp>
#include <mtk/core/types.hpp>
#include <mtk/core/zstring_view.hpp>

#include <string>

#ifndef MTK_FIXED_WIDTH_INTS
	#error System does not have fixed width integers (MTK_FIXED_WIDTH_INTS).
#endif

namespace mtk {

enum class inet_protocol
{
	ipv4,
	ipv6,
	unknown
};

struct ipv4_address
{
	uint32_t data;
};

struct ipv6_address
{
	uint32_t data[4];
	uint32_t flow;
	uint32_t scope;
};

class inet_address
{
public:
	inet_address() noexcept :
		m_proto(inet_protocol::unknown)
	{ }

	inet_address(const ipv4_address& ipv4) noexcept :
		m_proto(inet_protocol::ipv4),
		m_ipv4(ipv4)
	{ }

	inet_address(const ipv6_address& ipv6) noexcept :
		m_proto(inet_protocol::ipv6),
		m_ipv6(ipv6)
	{ }

	inet_address(zstring_view address, uint32_t flow = 0, uint32_t scope = 0) noexcept;



	inet_protocol
	protocol() const noexcept
	{
		return m_proto;
	}

	const ipv4_address&
	ipv4() const noexcept
	{
		MTK_ASSERT(m_proto == inet_protocol::ipv4);
		return m_ipv4;
	}

	const ipv6_address&
	ipv6() const noexcept
	{
		MTK_ASSERT(m_proto == inet_protocol::ipv6);
		return m_ipv6;
	}

	std::string
	to_string() const;



	bool
	is_loopback() const noexcept;

	bool
	is_multicast() const noexcept;


private:
	inet_protocol m_proto;
	union {
		ipv4_address m_ipv4;
		ipv6_address m_ipv6;
	};
};



bool
operator==(const inet_address& lhs, const inet_address& rhs) noexcept;

inline
bool
operator!=(const inet_address& lhs, const inet_address& rhs) noexcept
{
	return !(lhs == rhs);
}

} // namespace mtk

#endif
