#include <mtk/network/inet_address.hpp>

#include <mtk/core/byte_cast.hpp>
#include <mtk/network/impl/address.hpp>

#include <algorithm>
#include <cstring>

namespace mtk {

inet_address::inet_address(zstring_view address, uint32_t flow, uint32_t scope) noexcept :
	inet_address()
{
	mtk::_inet_addr addr;
	if (std::find(address.begin(), address.end(), ':') != address.end()) {
		if (mtk::_inet_pton(mtk::_af_inet6, address.data(), addr)) {
			m_proto = inet_protocol::ipv6;
			m_ipv6 = { };
			std::memcpy(&m_ipv6.data, addr.value, sizeof(m_ipv6.data));
			m_ipv6.flow = flow;
			m_ipv6.scope = scope;
		}
	} else if (std::find(address.begin(), address.end(), '.') != address.end()) {
		if (mtk::_inet_pton(mtk::_af_inet, address.data(), addr)) {
			m_proto = inet_protocol::ipv4;
			m_ipv4 = { };
			std::memcpy(&m_ipv4.data, addr.value, sizeof(m_ipv4.data));
		}
	}
}

std::string
inet_address::to_string() const
{
	mtk::_inet_addr addr;
	if (m_proto == inet_protocol::ipv4) {
		std::string ret(mtk::_inet_addrstrlen, '\0');
		std::memcpy(addr.value, &m_ipv4.data, sizeof(m_ipv4.data));
		if (mtk::_inet_ntop(mtk::_af_inet, addr,  ret.data(), ret.size()))
			return ret;

	} else if (m_proto == inet_protocol::ipv6) {
		std::string ret(mtk::_inet6_addrstrlen, '\0');
		std::memcpy(addr.value, &m_ipv6.data, sizeof(m_ipv6.data));
		if (mtk::_inet_ntop(mtk::_af_inet6, addr, ret.data(), ret.size()))
			return ret;
	}

	return "invalid inet_address";
}

bool
inet_address::is_loopback() const noexcept
{
	if (m_proto == inet_protocol::ipv4) {
		auto* bytes = mtk::byte_cast(&m_ipv4.data);
		return (bytes[0] == 0x7Fu) && (bytes[1] == 0u) && (bytes[2] == 0u);

	} else if (m_proto == inet_protocol::ipv6) {
		auto* bytes = mtk::byte_cast(&m_ipv6.data[3]);
		return (m_ipv6.data[0] == 0u) &&
			(m_ipv6.data[1] == 0u) &&
			(m_ipv6.data[2] == 0u) &&
			(bytes[0] == 0u) &&
			(bytes[1] == 0u) &&
			(bytes[2] == 0u) &&
			(bytes[3] == 1u);

	} else {
		return false;
	}
}

bool
inet_address::is_multicast() const noexcept
{
	if (m_proto == mtk::inet_protocol::ipv4) {
		auto* bytes = mtk::byte_cast(&m_ipv4.data);
		return ((bytes[0] & 0xF0u) == 0xE0u);

	} else if (m_proto == mtk::inet_protocol::ipv6) {
		auto* bytes = mtk::byte_cast(&m_ipv6.data);
		return bytes[0] == 0xFFu;

	} else {
		return false;
	}
}

bool
operator==(const inet_address& lhs, const inet_address& rhs) noexcept
{
	auto proto = lhs.protocol();
	if (proto != rhs.protocol())
		return false;

	if (proto == inet_protocol::ipv4) {
		return (lhs.ipv4().data == rhs.ipv4().data);

	} else if (proto == inet_protocol::ipv6) {
		const auto& lip = lhs.ipv6();
		const auto& rip = rhs.ipv6();
		return (std::memcmp(lip.data, rip.data, sizeof(lip.data)) == 0) &&
			(lip.flow == rip.flow) &&
			(lip.scope == rip.scope);

	} else {
		return true;
	}
}



} // namespace mtk
