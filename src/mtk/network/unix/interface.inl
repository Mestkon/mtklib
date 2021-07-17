#include <mtk/network/impl/interface.hpp>

#include <mtk/core/scope_exit.hpp>

#include <cstring>

#include <ifaddrs.h>
#include <net/if.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/types.h>

namespace mtk {

const int _iff_up = IFF_UP;
const int _iff_broadcast = IFF_BROADCAST;
const int _iff_loopback = IFF_LOOPBACK;
const int _iff_p2p = IFF_POINTOPOINT;
const int _iff_running = IFF_RUNNING;
const int _iff_multicast = IFF_MULTICAST;



std::list<_interface_entry>
_get_network_interfaces(bool& err)
{
	err = false;

	ifaddrs* addresses = nullptr;
	scope_exit addresses_cleanup = [addresses]() {
		if (addresses)
			::freeifaddrs(addresses);
	};

	int res = ::getifaddrs(&addresses);
	if (res < 0 || !addresses) {
		err = true;
		return { };
	}

	int flag_mask = (IFF_UP | IFF_BROADCAST | IFF_LOOPBACK | IFF_POINTOPOINT | IFF_RUNNING | IFF_MULTICAST);

	std::list<_interface_entry> ret;
	for (auto* addr = addresses; addr != nullptr; addr = addr->ifa_next) {
		if (!addr->ifa_addr || !addr->ifa_name)
			continue;

		size_t idx = ::if_nametoindex(addr->ifa_name);
		if (idx == 0)
			continue;

		_interface_entry entry;
		entry.name = std::string(addr->ifa_name);
		entry.index = idx;
		entry.flags = addr->ifa_flags & flag_mask;

		if (addr->ifa_addr) {
			entry.af = addr->ifa_addr->sa_family;
			if (entry.af == AF_INET) {
				sockaddr_in sa;
				std::memcpy(&sa, addr->ifa_addr, sizeof(sa));
				std::memcpy(&entry.address, &sa.sin_addr, sizeof(sa.sin_addr));
			} else if (entry.af == AF_INET6) {
				sockaddr_in6 sa;
				std::memcpy(&sa, addr->ifa_addr, sizeof(sa));
				entry.flow = sa.sin6_flowinfo;
				entry.scope = sa.sin6_scope_id;
				std::memcpy(&entry.address, &sa.sin6_addr, sizeof(sa.sin6_addr));
			} else {
				entry.af = AF_UNSPEC;
			}
		}

		ret.push_back(std::move(entry));
	}

	return ret;
}

} // namespace mtk
