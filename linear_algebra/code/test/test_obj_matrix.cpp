#include "linear_algebra.hpp"
#include <array>

using std::cout;
using std::endl;

using fsd_mat_engine_35 = STD_LA::fs_matrix_engine<double, 3, 5>;
using drd_mat_engine    = STD_LA::dr_matrix_engine<double, std::allocator<double>>;

using drm_double    = STD_LA::dyn_matrix<double>;
using drv_double    = STD_LA::dyn_vector<double>;

using fsv_double_5  = STD_LA::fs_vector<double, 5>;
using fsm_double_35 = STD_LA::fs_matrix<double, 3, 5>;
using fsm_float_35  = STD_LA::fs_matrix<float, 3, 5>;
using fsm_double_36 = STD_LA::fs_matrix<double, 3, 6>;

void t000()
{
    PRINT_FNAME();

    drm_double  m1; //(4, 4);

    Fill(m1);
    PRINT(m1);

    m1.resize(3, 3);
    Fill(m1);
    PRINT(m1);

    m1.reserve(6, 6);
    PRINT(m1);

    m1.resize(4, 4);
    PRINT(m1);

    drm_double  m2(m1);
    PRINT(m2);

    drm_double  m3(std::move(m1));
    PRINT(m1);
    PRINT(m3);
    Fill(m3);
    m3.resize(6, 6);
    PRINT(m3);

    fsm_double_35  fm1, fm2, fm3;
    Fill(fm1);
    PRINT(fm1);

    m1 = fm1;
    PRINT(m1);
    fm2 = fm1;
    PRINT(fm2);
    fm3 = m1;
    PRINT(fm3);

    PRINT(fm3.t());

    fsm_double_35   fm4;
    fsm_float_35    fm5;

    PRINT(fm4);
    Fill(fm5);
    PRINT(fm5);
    fm4 = fm5;
    PRINT(fm4);

    //  fm4 = fm5.t();              fails on static_assert, as expected
    //  fsm_double_36   fm6(fm4);   fails on static_assert, as expected
}

void t001()
{
    PRINT_FNAME();

    drm_double  m1(4, 5), m2;

    Fill(m1);
    PRINT(m1);
    PRINT(m1.t());
    PRINT(m1.column(1));

    PRINT(m1.row(1));
    PRINT(m1.t().column(1));

    auto    tc = m1.column(2);
    PRINT_TYPE(decltype(tc));
    PRINT(tc);
    PRINT_TYPE(decltype(tc(2)));
    tc(1) = 101;
    PRINT(tc);

    PRINT(m1.t().row(1));

    auto    tr = m1.row(2);
    PRINT_TYPE(decltype(tr));
    PRINT(tr);
    PRINT_TYPE(decltype(tr(2)));
    tr(3) = 101;
    PRINT(tr);

    PRINT(m1.t().t().column(1));
    PRINT(m1.t().t().row(1));

    std::cout << "\n================\n";

    PRINT(m1.column(0));
    PRINT(m1.column(1));
    PRINT(m1.column(2));
    PRINT(m1.column(3));
    PRINT(m1.column(4));

    PRINT(m1.row(0));
    PRINT(m1.row(1));
    PRINT(m1.row(2));
    PRINT(m1.row(3));

    PRINT(m1);
    m1.swap_columns(1, 3);
    PRINT(m1);
    m1.swap_rows(0, 2);
    PRINT(m1);

    m2 = m1;
    m2.swap_rows(0, 2);
    m2.swap_columns(1, 3);
    PRINT(m2);

    drv_double  v1;

    v1 = m2.row(1);
    PRINT(v1);
    v1 = m2.column(2);
    PRINT(v1);

    constexpr fsm_double_35     fsm = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    constexpr double            d = fsm(1,1);

    PRINT(fsm);

    auto    sm = fsm.submatrix(1, 2, 2, 3);
    PRINT(sm);
    //drm_double  mx = {1.0, 1.0, 1.0, 1.0};
}


constexpr double t002()
{
    fsm_double_35   fsm  = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    fsm_double_35   fsm2 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    fsv_double_5    fsv  = {11, 12, 13, 14, 15};
    fsv_double_5    fsv2 = {11, 12, 13};

    fsm.swap_columns(1, 3);
    fsm.swap_rows(0, 2);

    double    d1 = fsm(1,2);
    double    d2 = fsv(3);

    STD_LA::detail::la_swap(d1, d2);

    return d2;
}

constexpr double t003()
{
    constexpr double d = t002();
    return d;
}

constexpr double cd = t003();

using namespace std::experimental;
using namespace std::experimental::math;

#include <array>

using std::array;

namespace {
void t002X(drm_double const& m1, fsm_double_36 const& m2)
{
#ifdef LA_USE_MDSPAN
    auto&   eng = m1.engine();
    auto    spn = eng.span();
    PRINT(spn);
    PRINT_TYPE(decltype(spn));

    auto&   eng2 = m2.engine();
    auto    spn2 = eng2.span();
    PRINT(spn2);
    PRINT_TYPE(decltype(spn2));

    auto    tr3  = m1.t();
    PRINT(tr3);
    PRINT_TYPE(decltype(tr3));
    PRINT_TYPE(typename decltype(tr3)::engine_type::span_type);
    PRINT_TYPE(typename decltype(tr3)::engine_type::const_span_type);

    auto&   eng3 = tr3.engine();
    auto    spn3 = eng3.span();
    PRINT(spn3);
    PRINT_TYPE(decltype(spn3));

    auto    tr4  = m2.t();
    PRINT(tr4);
    PRINT_TYPE(decltype(tr4));

    auto&   eng4 = tr4.engine();
    auto    spn4 = eng4.span();
    PRINT(spn4);
    PRINT_TYPE(decltype(spn4));

    auto    col1 = m1.column(1);
    auto&   ceng1 = col1.engine();
    PRINT(col1);
    PRINT_TYPE(decltype(col1));

    auto    cspn1 = ceng1.span();
    PRINT(cspn1);
    PRINT_TYPE(decltype(cspn1));

    auto    row1 = m1.row(1);
    auto&   reng1 = row1.engine();
    PRINT(row1);
    PRINT_TYPE(decltype(row1));

    auto    rspn1 = reng1.span();
    PRINT(rspn1);
    PRINT_TYPE(decltype(rspn1));

    auto    sub1 = m1.submatrix(2, 4, 3, 6);
    PRINT(sub1);
    PRINT_TYPE(decltype(sub1));

    auto    sspn1 = sub1.span();
    PRINT(sspn1);
    PRINT_TYPE(decltype(sspn1));

    auto    subtr1 = m1.submatrix(2,4,3,6).t();
    PRINT(subtr1);
    PRINT_TYPE(decltype(subtr1));

    auto    subtrspn1 = subtr1.span();
    PRINT(subtrspn1);
    PRINT_TYPE(decltype(subtrspn1));
#endif
}

void t001X()
{
#ifdef LA_USE_MDSPAN
    drm_double  m1(10, 13, 16, 19);
    Fill(m1);
    PRINT(m1);

    auto    spn1 = m1.span();
    PRINT(spn1);
    PRINT_TYPE(decltype(spn1));

    fsm_double_36  m2;
    Fill(m2);
    PRINT(m2);

    auto    spn2 = m2.span();
    PRINT(spn2);
    PRINT_TYPE(decltype(spn2));

    auto    tr3  = m1.t();
    PRINT(tr3);
    PRINT_TYPE(decltype(tr3));

    auto    spn3 = tr3.span();
    PRINT(spn3);
    PRINT_TYPE(decltype(spn3));

    auto    tr4  = m2.t();
    PRINT(tr4);
    PRINT_TYPE(decltype(tr4));

    auto    spn4 = tr4.span();
    PRINT(spn4);
    PRINT_TYPE(decltype(spn4));

    auto    col1 = m1.column(1);
    PRINT(col1);
    PRINT_TYPE(decltype(col1));

    auto    cspn1 = col1.span();
    PRINT(cspn1);
    PRINT_TYPE(decltype(cspn1));

    auto    row1 = m1.row(1);
    PRINT(row1);
    PRINT_TYPE(decltype(row1));

    auto    rspn1 = row1.span();
    PRINT(rspn1);
    PRINT_TYPE(decltype(rspn1));

    auto    sub1 = m1.submatrix(2, 4, 3, 6);
    PRINT(sub1);
    PRINT_TYPE(decltype(sub1));

    auto    sspn1 = sub1.span();
    PRINT(sspn1);
    PRINT_TYPE(decltype(sspn1));

    t002X(m1, m2);
#endif
}
}

void
TestGroup00()
{
    constexpr double x = t002();

    PRINT_FNAME();

    t001X();

//    t000();
//    t001();
}
