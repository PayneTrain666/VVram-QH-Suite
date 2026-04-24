#include <cstdint>
#include <vector>
namespace holoram {
std::vector<std::uint8_t> compute_xor_residual(const std::uint8_t* original,const std::uint8_t* reconstructed,std::uint32_t n){std::vector<std::uint8_t> residual(n); for(std::uint32_t i=0;i<n;++i) residual[i]=static_cast<std::uint8_t>(original[i]^reconstructed[i]); return residual;}
std::vector<std::uint8_t> apply_xor_residual(const std::uint8_t* reconstructed,const std::uint8_t* residual,std::uint32_t n){std::vector<std::uint8_t> exact(n); for(std::uint32_t i=0;i<n;++i) exact[i]=static_cast<std::uint8_t>(reconstructed[i]^residual[i]); return exact;}
}
