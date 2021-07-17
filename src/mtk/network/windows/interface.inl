#include <mtk/network/impl/interface.hpp>

#include <mtk/core/assert.hpp>

#include <cstring>
#include <memory>

#include <winsock2.h>
#include <iphlpapi.h>
#include <iptypes.h>
#include <Ws2tcpip.h>

namespace mtk {

const int _iff_up = 1;
const int _iff_broadcast = 2;
const int _iff_loopback = 4;
const int _iff_p2p = 8;
const int _iff_running = 16;
const int _iff_multicast = 32;


constexpr int max_address_tries = 5;

std::list<_interface_entry>
_get_network_interfaces(bool& err)
{
	err = false;

	std::unique_ptr<IP_ADAPTER_ADDRESSES[]> addresses;
	ULONG size = 0;

	ULONG flags = GAA_FLAG_SKIP_MULTICAST | GAA_FLAG_SKIP_DNS_SERVER | GAA_FLAG_SKIP_FRIENDLY_NAME;
	ULONG res = ERROR_SUCCESS;
	int tries = 0;
	while ((res = ::GetAdaptersAddresses(AF_UNSPEC, flags, nullptr, addresses.get(), &size)) == ERROR_BUFFER_OVERFLOW) {
		if (tries++ >= max_address_tries) {
			err = true;
			return { };
		}

		MTK_ASSERT(size != 0);
		ULONG new_size = size / sizeof(IP_ADAPTER_ADDRESSES) + 1;
		size = new_size*sizeof(IP_ADAPTER_ADDRESSES);
		addresses = std::make_unique<IP_ADAPTER_ADDRESSES[]>(new_size);
	}

	if (res != ERROR_SUCCESS) {
		err = true;
		return { };
	}

	std::list<_interface_entry> ret;
	for (auto* iface = addresses.get(); iface != nullptr; iface = iface->Next) {
		if (!iface->AdapterName || !iface->FirstUnicastAddress)
			continue;

		_interface_entry entry;
		entry.name = std::string(iface->AdapterName);
		if (iface->IfIndex != 0)
			entry.index = iface->IfIndex;
		else
			entry.index = iface->Ipv6IfIndex;
		entry.flags = 0;

		if (iface->OperStatus == IfOperStatusUp)
			entry.flags |= _iff_up | _iff_running;

		entry.flags = _iff_broadcast;

		if (iface->IfType == IF_TYPE_SOFTWARE_LOOPBACK)
			entry.flags |= _iff_loopback;
		else if (iface->IfType == IF_TYPE_PPP)
			entry.flags |= _iff_p2p;

		if (!iface->NoMulticast)
			entry.flags |= _iff_multicast;

		for (auto* addr = iface->FirstUnicastAddress; addr != nullptr; addr = addr->Next) {
			if (!addr->Address.lpSockaddr)
				continue;

			_interface_entry cp = entry;
			const auto& native_addr = addr->Address.lpSockaddr;
			cp.af = native_addr->sa_family;
			if (cp.af == AF_INET) {
				sockaddr_in sa;
				std::memcpy(&sa, native_addr, sizeof(sa));
				std::memcpy(&cp.address, &sa.sin_addr, sizeof(sa.sin_addr));
			} else if (cp.af == AF_INET6) {
				sockaddr_in6 sa;
				std::memcpy(&sa, native_addr, sizeof(sa));
				cp.flow = sa.sin6_flowinfo;
				cp.scope = sa.sin6_scope_id;
				std::memcpy(&cp.address, &sa.sin6_addr, sizeof(sa.sin6_addr));
			} else {
				cp.af = AF_UNSPEC;
			}

			ret.push_back(cp);
		}

		for (auto* addr = iface->FirstAnycastAddress; addr != nullptr; addr = addr->Next) {
			if (!addr->Address.lpSockaddr)
				continue;

			_interface_entry cp = entry;
			const auto& native_addr = addr->Address.lpSockaddr;
			cp.af = native_addr->sa_family;
			if (cp.af == AF_INET) {
				sockaddr_in sa;
				std::memcpy(&sa, native_addr, sizeof(sa));
				std::memcpy(&cp.address, &sa.sin_addr, sizeof(sa.sin_addr));
			} else if (cp.af == AF_INET6) {
				sockaddr_in6 sa;
				std::memcpy(&sa, native_addr, sizeof(sa));
				cp.flow = sa.sin6_flowinfo;
				cp.scope = sa.sin6_scope_id;
				std::memcpy(&cp.address, &sa.sin6_addr, sizeof(sa.sin6_addr));
			} else {
				cp.af = AF_UNSPEC;
			}

			ret.push_back(cp);
		}
	}

	return ret;
}

} // namespace mtk
