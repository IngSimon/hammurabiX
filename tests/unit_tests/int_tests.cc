#include <gtest/gtest.h>

#include <cgs_units_file.h>
#include <cmath>
#include <grid.h>
#include <integrator.h>
#include <memory>

TEST(integrator, boundary_check) {
  Integrator test;
  double R0 = 10.;
  double R_lim = (1.0 + 1.0e-5) * R0;
  EXPECT_FALSE(test.check_simulation_upper_limit(R0, R_lim));
  EXPECT_TRUE(test.check_simulation_lower_limit(R0, R_lim));
}

TEST(integrator, shell_info_assembling) {
  unsigned int test_unsigned;
  double test_double;
  auto prop = std::make_unique<Integrator>();
  auto ref = std::make_unique<Integrator::struct_shell>();
  //
  auto par = std::make_unique<Param>("reference/int_tests_01.xml");
  //
  test_unsigned = 2;
  EXPECT_EQ(par->grid_int.sim_sync_freq.size(), test_unsigned);
  test_double = 23.0 * CGS_U_GHz;
  EXPECT_EQ(par->grid_int.sim_sync_freq[0], test_double);
  test_double = 1.4 * CGS_U_GHz;
  EXPECT_EQ(par->grid_int.sim_sync_freq[1], test_double);
  test_unsigned = 32;
  EXPECT_EQ(par->grid_int.nside_shell[0], test_unsigned);
  test_unsigned = 64;
  EXPECT_EQ(par->grid_int.nside_shell[1], test_unsigned);
  test_unsigned = 128;
  EXPECT_EQ(par->grid_int.nside_shell[2], test_unsigned);
  //
  prop->assemble_shell_ref(ref.get(), par.get(), 1);
  test_unsigned = 333;
  EXPECT_EQ(ref->step, test_unsigned);
  test_double = 5 * CGS_U_kpc;
  EXPECT_EQ(ref->d_start, test_double);
  test_double = 40 * CGS_U_kpc / 4 + 5 * CGS_U_kpc;
  EXPECT_EQ(ref->d_stop, test_double);
  //
  prop->assemble_shell_ref(ref.get(), par.get(), 3);
  test_unsigned = 666;
  EXPECT_EQ(ref->step, test_unsigned);
  test_double = 40 * CGS_U_kpc / 2 + 5 * CGS_U_kpc;
  EXPECT_EQ(ref->d_start, test_double);
  test_double = 45 * CGS_U_kpc;
  EXPECT_EQ(ref->d_stop, test_double);
  //
  par = std::make_unique<Param>("reference/int_tests_02.xml");
  //
  test_unsigned = 32;
  EXPECT_EQ(par->grid_int.nside_shell[0], test_unsigned);
  test_unsigned = 16;
  EXPECT_EQ(par->grid_int.nside_shell[1], test_unsigned);
  test_unsigned = 8;
  EXPECT_EQ(par->grid_int.nside_shell[2], test_unsigned);
  //
  prop->assemble_shell_ref(ref.get(), par.get(), 1);
  test_unsigned = 100;
  EXPECT_EQ(ref->step, test_unsigned);
  test_double = 0.;
  EXPECT_EQ(ref->d_start, test_double);
  test_double = par->grid_int.ec_r_max * 0.1;
  EXPECT_EQ(ref->d_stop, test_double);
  //
  prop->assemble_shell_ref(ref.get(), par.get(), 2);
  test_unsigned = 600;
  EXPECT_EQ(ref->step, test_unsigned);
  test_double = par->grid_int.ec_r_max * 0.1;
  EXPECT_EQ(ref->d_start, test_double);
  test_double = par->grid_int.ec_r_max * 0.7;
  EXPECT_EQ(ref->d_stop, test_double);
  //
  prop->assemble_shell_ref(ref.get(), par.get(), 3);
  test_unsigned = 300;
  EXPECT_EQ(ref->step, test_unsigned);
  test_double = par->grid_int.ec_r_max * 0.7;
  EXPECT_EQ(ref->d_start, test_double);
  test_double = par->grid_int.ec_r_max * 1.0;
  EXPECT_EQ(ref->d_stop, test_double);
}
