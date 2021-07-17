#include <mtk/network/network_interface.hpp>

#include <mtk/network/impl/address.hpp>
#include <mtk/network/impl/interface.hpp>

#include <algorithm>
#include <cstring>

namespace mtk {

array<network_interface>
all_network_interfaces(bool* error) noexcept
{
	if (error)
		*error = false;

	try {
		array<network_interface> ret;

		bool err = false;
		auto ifaces = mtk::_get_network_interfaces(err);
		if (err) {
			if (error)
				*error = true;

			return { };
		}

		for (auto& iface : ifaces) {
			auto it = std::find_if(ret.begin(), ret.end(), [&iface] (const network_interface& nw_iface) {
				return iface.index == nw_iface.m_index;
			});

			if (it == ret.end()) {
				ret.resize(ret.size() + 1);

				it = ret.end() - 1;
				it->m_name = std::move(iface.name);
				it->m_index = iface.index;

				if (iface.flags & _iff_up)
					it->m_flags |= network_interface_flag::up;
				if (iface.flags & _iff_running)
					it->m_flags |= network_interface_flag::running;
				if (iface.flags & _iff_loopback)
					it->m_flags |= network_interface_flag::loopback;
				if (iface.flags & _iff_broadcast)
					it->m_flags |= network_interface_flag::broadcast;
				if (iface.flags & _iff_multicast)
					it->m_flags |= network_interface_flag::multicast;
				if (iface.flags & _iff_p2p)
					it->m_flags |= network_interface_flag::point_to_point;

			}

			if (iface.af == _af_inet) {
				ipv4_address addr;
				std::memcpy(&addr.data, &iface.address, sizeof(addr.data));
				it->m_addresses.resize(it->m_addresses.size() + 1);
				it->m_addresses.back() = addr;
			} else if (iface.af == _af_inet6) {
				ipv6_address addr;
				std::memcpy(&addr.data, &iface.address, sizeof(addr.data));
				addr.flow = iface.flow;
				addr.scope = iface.scope;
				it->m_addresses.resize(it->m_addresses.size() + 1);
				it->m_addresses.back() = addr;
			}
		}

		return ret;

	} catch (...) {
		if (error)
			*error = true;

		return { };
	}
}

} // namespace mtk
