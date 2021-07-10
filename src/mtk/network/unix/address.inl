#include <mtk/network/impl/address.hpp>

#include <mtk/core/assert.hpp>
#include <mtk/core/saturate_cast.hpp>

#include <arpa/inet.h>
#include <netinet/in.h>

namespace mtk {

const int _af_inet = AF_INET;
const int _af_inet6 = AF_INET6;

const int _inet_addrstrlen = INET_ADDRSTRLEN;
const int _inet6_addrstrlen = INET6_ADDRSTRLEN;

bool _inet_pton(int af, const char* src, _inet_addr& dst) noexcept
{
	MTK_ASSERT((af == AF_INET) || (af == AF_INET6));
	MTK_ASSERT(src);

	return ::inet_pton(af, src, dst.value) == 1;
}

bool _inet_ntop(int af, const _inet_addr& src, char* dst, size_t size) noexcept
{
	MTK_ASSERT((af == AF_INET) || (af == AF_INET6));
	MTK_ASSERT(dst);

	return ::inet_ntop(af, src.value, dst, mtk::saturate_cast<socklen_t>(size)) != nullptr;
}



} // namespace mtk
