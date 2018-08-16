/* ************************************************************************
 * Copyright 2018 Advanced Micro Devices, Inc.
 * ************************************************************************ */

#include "testing_csr2ell.hpp"
#include "utility.hpp"

#include <rocsparse.h>
#include <gtest/gtest.h>
#include <vector>
#include <string>

typedef std::tuple<int, int, rocsparse_index_base, rocsparse_index_base> csr2ell_tuple;
typedef std::tuple<rocsparse_index_base, rocsparse_index_base, std::string> csr2ell_bin_tuple;

int csr2ell_M_range[] = {-1, 0, 10, 500, 872, 1000};
int csr2ell_N_range[] = {-3, 0, 33, 242, 623, 1000};

rocsparse_index_base csr2ell_csr_base_range[] = {rocsparse_index_base_zero,
                                                 rocsparse_index_base_one};
rocsparse_index_base csr2ell_ell_base_range[] = {rocsparse_index_base_zero,
                                                 rocsparse_index_base_one};

std::string csr2ell_bin[] = {"rma10.bin",
                             "mac_econ_fwd500.bin",
                             "bibd_22_8.bin",
                             "mc2depi.bin",
                             "scircuit.bin",
                             "bmwcra_1.bin",
                             "nos1.bin",
                             "nos2.bin",
                             "nos3.bin",
                             "nos4.bin",
                             "nos5.bin",
                             "nos6.bin",
                             "nos7.bin"};

class parameterized_csr2ell : public testing::TestWithParam<csr2ell_tuple>
{
    protected:
    parameterized_csr2ell() {}
    virtual ~parameterized_csr2ell() {}
    virtual void SetUp() {}
    virtual void TearDown() {}
};

class parameterized_csr2ell_bin : public testing::TestWithParam<csr2ell_bin_tuple>
{
    protected:
    parameterized_csr2ell_bin() {}
    virtual ~parameterized_csr2ell_bin() {}
    virtual void SetUp() {}
    virtual void TearDown() {}
};

Arguments setup_csr2ell_arguments(csr2ell_tuple tup)
{
    Arguments arg;
    arg.M         = std::get<0>(tup);
    arg.N         = std::get<1>(tup);
    arg.idx_base  = std::get<2>(tup);
    arg.idx_base2 = std::get<3>(tup);
    arg.timing    = 0;
    return arg;
}

Arguments setup_csr2ell_arguments(csr2ell_bin_tuple tup)
{
    Arguments arg;
    arg.M         = -99;
    arg.N         = -99;
    arg.idx_base  = std::get<0>(tup);
    arg.idx_base2 = std::get<1>(tup);
    arg.timing    = 0;

    // Determine absolute path of test matrix
    std::string bin_file = std::get<2>(tup);

    // Get current executables absolute path
    char path_exe[PATH_MAX];
    ssize_t len = readlink("/proc/self/exe", path_exe, sizeof(path_exe) - 1);
    if(len < 14)
    {
        path_exe[0] = '\0';
    }
    else
    {
        path_exe[len - 14] = '\0';
    }

    // Matrices are stored at the same path in matrices directory
    arg.filename = std::string(path_exe) + "matrices/" + bin_file;

    return arg;
}

TEST(csr2ell_bad_arg, csr2ell) { testing_csr2ell_bad_arg<float>(); }

TEST_P(parameterized_csr2ell, csr2ell_float)
{
    Arguments arg = setup_csr2ell_arguments(GetParam());

    rocsparse_status status = testing_csr2ell<float>(arg);
    EXPECT_EQ(status, rocsparse_status_success);
}

TEST_P(parameterized_csr2ell, csr2ell_double)
{
    Arguments arg = setup_csr2ell_arguments(GetParam());

    rocsparse_status status = testing_csr2ell<double>(arg);
    EXPECT_EQ(status, rocsparse_status_success);
}

TEST_P(parameterized_csr2ell_bin, csr2ell_bin_float)
{
    Arguments arg = setup_csr2ell_arguments(GetParam());

    rocsparse_status status = testing_csr2ell<float>(arg);
    EXPECT_EQ(status, rocsparse_status_success);
}

TEST_P(parameterized_csr2ell_bin, csr2ell_bin_double)
{
    Arguments arg = setup_csr2ell_arguments(GetParam());

    rocsparse_status status = testing_csr2ell<double>(arg);
    EXPECT_EQ(status, rocsparse_status_success);
}

INSTANTIATE_TEST_CASE_P(csr2ell,
                        parameterized_csr2ell,
                        testing::Combine(testing::ValuesIn(csr2ell_M_range),
                                         testing::ValuesIn(csr2ell_N_range),
                                         testing::ValuesIn(csr2ell_csr_base_range),
                                         testing::ValuesIn(csr2ell_ell_base_range)));

INSTANTIATE_TEST_CASE_P(csr2ell_bin,
                        parameterized_csr2ell_bin,
                        testing::Combine(testing::ValuesIn(csr2ell_csr_base_range),
                                         testing::ValuesIn(csr2ell_ell_base_range),
                                         testing::ValuesIn(csr2ell_bin)));
