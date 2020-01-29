//==================================================================================================
//  File:       matrix.hpp
//
//  Summary:    This header defines the matrix class template, one of the two primary math
//              objects provided by the library.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_MATRIX_HPP_DEFINED
#define LINEAR_ALGEBRA_MATRIX_HPP_DEFINED

#include <linear_algebra.hpp>

namespace STD_LA {
//==================================================================================================
//  A matrix parametrized by an engine type and operator traits.
//==================================================================================================
//
template<class ET, class OT>
class matrix
{
    static_assert(is_matrix_engine_v<ET>);
#ifdef LA_USE_MDSPAN
    static_assert(detail::has_valid_span_alias_form_v<ET>);
#endif
    using possibly_writable_vector_tag = detail::noe_category_t<ET, writable_vector_engine_tag>;
    using possibly_writable_matrix_tag = detail::noe_category_t<ET, writable_matrix_engine_tag>;

    static constexpr bool   has_cx_elem  = detail::is_complex_v<typename ET::value_type>;

  public:
    //- Types
    //
    using engine_type          = ET;
    using element_type         = typename engine_type::element_type;
    using value_type           = typename engine_type::value_type;
    using reference            = typename engine_type::reference;
    using const_reference      = typename engine_type::const_reference;
    using difference_type      = typename engine_type::difference_type;
    using size_type            = typename engine_type::size_type;
    using size_tuple           = typename engine_type::size_tuple;

    using column_type          = vector<column_engine<engine_type, possibly_writable_vector_tag>, OT>;
    using const_column_type    = vector<column_engine<engine_type, readable_vector_engine_tag>, OT>;
    using row_type             = vector<row_engine<engine_type, possibly_writable_vector_tag>, OT>;
    using const_row_type       = vector<row_engine<engine_type, readable_vector_engine_tag>, OT>;
    using submatrix_type       = matrix<submatrix_engine<engine_type, possibly_writable_matrix_tag>, OT>;
    using const_submatrix_type = matrix<submatrix_engine<engine_type, readable_matrix_engine_tag>, OT>;
    using transpose_type       = matrix<transpose_engine<engine_type, possibly_writable_matrix_tag>, OT>;
    using const_transpose_type = matrix<transpose_engine<engine_type, readable_matrix_engine_tag>, OT>;
    using hermitian_type       = conditional_t<has_cx_elem, matrix, transpose_type>;
    using const_hermitian_type = conditional_t<has_cx_elem, matrix, const_transpose_type>;

#ifdef LA_USE_MDSPAN
    using span_type            = detail::engine_span_t<ET>;
    using const_span_type      = detail::engine_const_span_t<ET>;
#endif

    //- Construct/copy/destroy
    //
    ~matrix() noexcept = default;

    constexpr matrix() = default;
    constexpr matrix(matrix&&) noexcept = default;
    constexpr matrix(matrix const&) = default;

    template<class ET2, class OT2>
    constexpr matrix(matrix<ET2, OT2> const& src);
    template<class U, class ET2 = ET, detail::enable_if_initable<ET, ET2> = true>
    constexpr matrix(initializer_list<initializer_list<U>> rhs);

    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    constexpr matrix(size_tuple size);
    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    constexpr matrix(size_type rows, size_type cols);
    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    constexpr matrix(size_tuple size, size_tuple cap);
    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    constexpr matrix(size_type rows, size_type cols, size_type rowcap, size_type colcap);

    constexpr matrix&   operator =(matrix&&) noexcept = default;
    constexpr matrix&   operator =(matrix const&) = default;
    template<class ET2, class OT2>
    constexpr matrix&   operator =(matrix<ET2, OT2> const& rhs);
    template<class U, class ET2 = ET, detail::enable_if_writable<ET, ET2> = true>
    constexpr matrix&   operator =(initializer_list<initializer_list<U>> rhs);

    //- Capacity
    //
    static constexpr bool   is_resizable() noexcept;
    constexpr size_type     columns() const noexcept;
    constexpr size_type     rows() const noexcept;
    constexpr size_tuple    size() const noexcept;

    constexpr size_type     column_capacity() const noexcept;
    constexpr size_type     row_capacity() const noexcept;
    constexpr size_tuple    capacity() const noexcept;

    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    constexpr void      reserve(size_tuple cap);
    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    constexpr void      reserve(size_type rowcap, size_type colcap);

    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    constexpr void      resize(size_tuple size);
    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    constexpr void      resize(size_type rows, size_type cols);

    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    constexpr void      resize(size_tuple size, size_tuple cap);
    template<class ET2 = ET, detail::enable_if_resizable<ET, ET2> = true>
    constexpr void      resize(size_type rows, size_type cols, size_type rowcap, size_type colcap);

    //- Element access
    //
    constexpr reference             operator ()(size_type i, size_type j);
    constexpr const_reference       operator ()(size_type i, size_type j) const;

    constexpr column_type           column(size_type j) noexcept;
    constexpr const_column_type     column(size_type j) const noexcept;
    constexpr row_type              row(size_type i) noexcept;
    constexpr const_row_type        row(size_type i) const noexcept;
    constexpr submatrix_type        submatrix(size_type ri, size_type rn, size_type ci, size_type cn) noexcept;
    constexpr const_submatrix_type  submatrix(size_type ri, size_type rn, size_type ci, size_type cn) const noexcept;
    constexpr transpose_type        t() noexcept;
    constexpr const_transpose_type  t() const noexcept;
    constexpr hermitian_type        h();
    constexpr const_hermitian_type  h() const;

    //- Data access
    //
    constexpr engine_type&          engine() noexcept;
    constexpr engine_type const&    engine() const noexcept;

#ifdef LA_USE_MDSPAN
    template<class ET2 = ET, detail::enable_if_spannable<ET, ET2> = true>
    constexpr span_type             span() noexcept;
    template<class ET2 = ET, detail::enable_if_spannable<ET, ET2> = true>
    constexpr const_span_type       span() const noexcept;
#endif

    //- Modifiers
    //
    constexpr void      swap(matrix& rhs) noexcept;
    template<class ET2 = ET, detail::enable_if_writable<ET, ET2> = true>
    constexpr void      swap_columns(size_type i, size_type j) noexcept;
    template<class ET2 = ET, detail::enable_if_writable<ET, ET2> = true>
    constexpr void      swap_rows(size_type i, size_type j) noexcept;

  private:
    template<class ET2, class OT2> friend class matrix;
    template<class ET2, class OT2> friend class vector;

    engine_type     m_engine;

    template<class ET2, class... ARGS>
    constexpr matrix(detail::special_ctor_tag, ET2&& eng, ARGS&&... args);
};

//------------------------
//- Construct/copy/destroy
//
template<class ET, class OT>
template<class ET2, class OT2> constexpr
matrix<ET,OT>::matrix(matrix<ET2, OT2> const& rhs)
:   m_engine()
{
    assign(rhs);
}

template<class ET, class OT>
template<class U, class ET2, detail::enable_if_initable<ET, ET2>> constexpr
matrix<ET,OT>::matrix(initializer_list<initializer_list<U>> rhs)
:   m_engine(rhs)
{}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>> constexpr
matrix<ET,OT>::matrix(size_tuple size)
:   m_engine(get<0>(size), get<1>(size))
{}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>> constexpr
matrix<ET,OT>::matrix(size_type rows, size_type cols)
:   m_engine(rows, cols)
{}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>> constexpr
matrix<ET,OT>::matrix(size_tuple size, size_tuple cap)
:   m_engine(get<0>(size), get<1>(size), get<0>(cap), get<1>(cap))
{}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>> constexpr
matrix<ET,OT>::matrix(size_type rows, size_type cols, size_type rowcap, size_type colcap)
:   m_engine(rows, cols, rowcap, colcap)
{}

template<class ET, class OT>
template<class ET2, class... ARGS> constexpr
matrix<ET,OT>::matrix(detail::special_ctor_tag, ET2&& eng, ARGS&&... args)
:   m_engine(std::forward<ET2>(eng), std::forward<ARGS>(args)...)
{}

template<class ET, class OT>
template<class ET2, class OT2> constexpr
matrix<ET,OT>&
matrix<ET,OT>::operator =(matrix<ET2, OT2> const& rhs)
{
    m_engine = rhs.m_engine;
    return *this;
}

template<class ET, class OT>
template<class U, class ET2, detail::enable_if_writable<ET, ET2>> constexpr
matrix<ET,OT>&
matrix<ET,OT>::operator =(initializer_list<initializer_list<U>> rhs)
{
    m_engine = rhs;
    return *this;
}

//----------
//- Capacity
//
template<class ET, class OT> constexpr
bool
matrix<ET,OT>::is_resizable() noexcept
{
    return is_resizable_engine_v<ET>;
}

template<class ET, class OT> inline constexpr
typename matrix<ET,OT>::size_type
matrix<ET,OT>::columns() const noexcept
{
    return m_engine.columns();
}

template<class ET, class OT> inline constexpr
typename matrix<ET,OT>::size_type
matrix<ET,OT>::rows() const noexcept
{
    return m_engine.rows();
}

template<class ET, class OT> inline constexpr
typename matrix<ET,OT>::size_tuple
matrix<ET,OT>::size() const noexcept
{
    return size_tuple(m_engine.rows(), m_engine.columns());
}

template<class ET, class OT> inline constexpr
typename matrix<ET,OT>::size_type
matrix<ET,OT>::column_capacity() const noexcept
{
    return m_engine.column_capacity();
}

template<class ET, class OT> inline constexpr
typename matrix<ET,OT>::size_type
matrix<ET,OT>::row_capacity() const noexcept
{
    return m_engine.row_capacity();
}

template<class ET, class OT> inline constexpr
typename matrix<ET,OT>::size_tuple
matrix<ET,OT>::capacity() const noexcept
{
    return size_tuple(m_engine.row_capacity(), m_engine.column_capacity());
}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>> inline constexpr
void
matrix<ET,OT>::reserve(size_tuple cap)
{
    m_engine.resize(get<0>(cap), get<1>(cap));
}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>> inline constexpr
void
matrix<ET,OT>::reserve(size_type rowcap, size_type colcap)
{
    m_engine.reserve(rowcap, colcap);
}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>> inline constexpr
void
matrix<ET,OT>::resize(size_tuple size)
{
    m_engine.resize(get<0>(size), get<1>(size));
}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>> inline constexpr
void
matrix<ET,OT>::resize(size_type rows, size_type cols)
{
    m_engine.resize(rows, cols);
}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>> inline constexpr
void
matrix<ET,OT>::resize(size_tuple size, size_tuple cap)
{
    m_engine.resize(get<0>(size), get<1>(size), get<0>(cap), get<1>(cap));
}

template<class ET, class OT>
template<class ET2, detail::enable_if_resizable<ET, ET2>> inline constexpr
void
matrix<ET,OT>::resize(size_type rows, size_type cols, size_type rowcap, size_type colcap)
{
    m_engine.resize(rows, cols, rowcap, colcap);
}

//----------------
//- Element access
//
template<class ET, class OT> inline constexpr
typename matrix<ET,OT>::reference
matrix<ET,OT>::operator ()(size_type i, size_type j)
{
    return m_engine(i, j);
}

template<class ET, class OT> inline constexpr
typename matrix<ET,OT>::const_reference
matrix<ET,OT>::operator ()(size_type i, size_type j) const
{
    return m_engine(i, j);
}

template<class ET, class OT> inline constexpr
typename matrix<ET,OT>::const_column_type
matrix<ET,OT>::column(size_type j) const noexcept
{
    return const_column_type(detail::special_ctor_tag(), m_engine, j);
}

template<class ET, class OT> inline constexpr
typename matrix<ET,OT>::column_type
matrix<ET,OT>::column(size_type j) noexcept
{
    return column_type(detail::special_ctor_tag(), m_engine, j);
}

template<class ET, class OT> inline constexpr
typename matrix<ET,OT>::row_type
matrix<ET,OT>::row(size_type i) noexcept
{
    return row_type(detail::special_ctor_tag(), m_engine, i);
}

template<class ET, class OT> inline constexpr
typename matrix<ET,OT>::const_row_type
matrix<ET,OT>::row(size_type i) const noexcept
{
    return const_row_type(detail::special_ctor_tag(), m_engine, i);
}

template<class ET, class OT> inline constexpr
typename matrix<ET,OT>::submatrix_type
matrix<ET,OT>::submatrix(size_type ri, size_type rn, size_type ci, size_type cn) noexcept
{
    return submatrix_type(detail::special_ctor_tag(), m_engine, ri, rn, ci, cn);
}

template<class ET, class OT> inline constexpr
typename matrix<ET,OT>::const_submatrix_type
matrix<ET,OT>::submatrix(size_type ri, size_type rn, size_type ci, size_type cn) const noexcept
{
    return const_submatrix_type(detail::special_ctor_tag(), m_engine, ri, rn, ci, cn);
}

template<class ET, class OT> inline constexpr
typename matrix<ET,OT>::transpose_type
matrix<ET,OT>::t() noexcept
{
    return transpose_type(detail::special_ctor_tag(), m_engine);
}

template<class ET, class OT> inline constexpr
typename matrix<ET,OT>::const_transpose_type
matrix<ET,OT>::t() const noexcept
{
    return const_transpose_type(detail::special_ctor_tag(), m_engine);
}

template<class ET, class OT> inline constexpr
typename matrix<ET,OT>::hermitian_type
matrix<ET,OT>::h()
{
    if constexpr (detail::is_complex_v<element_type>)
    {
        return hermitian_type();
    }
    else
    {
        return hermitian_type(detail::special_ctor_tag(), m_engine);
    }
}

template<class ET, class OT> inline constexpr
typename matrix<ET,OT>::const_hermitian_type
matrix<ET,OT>::h() const
{
    if constexpr (detail::is_complex_v<element_type>)
    {
        return const_hermitian_type();
    }
    else
    {
        return const_hermitian_type(detail::special_ctor_tag(), m_engine);
    }
}

//-------------
//- Data access
//
template<class ET, class OT> constexpr
typename matrix<ET,OT>::engine_type&
matrix<ET,OT>::engine() noexcept
{
    return m_engine;
}

template<class ET, class OT> constexpr
typename matrix<ET,OT>::engine_type const&
matrix<ET,OT>::engine() const noexcept
{
    return m_engine;
}

#ifdef LA_USE_MDSPAN

template<class ET, class OT>
template<class ET2, detail::enable_if_spannable<ET, ET2>> inline constexpr
typename matrix<ET,OT>::span_type
matrix<ET,OT>::span() noexcept
{
    return m_engine.span();
}

template<class ET, class OT>
template<class ET2, detail::enable_if_spannable<ET, ET2>> inline constexpr
typename matrix<ET,OT>::const_span_type
matrix<ET,OT>::span() const noexcept
{
    return m_engine.span();
}

#endif

//-----------
//- Modifiers
//
template<class ET, class OT> constexpr
void
matrix<ET,OT>::swap(matrix& rhs) noexcept
{
    m_engine.swap(rhs.m_engine);
}

template<class ET, class OT>
template<class ET2, detail::enable_if_writable<ET, ET2>> inline constexpr
void
matrix<ET,OT>::swap_columns(size_type c1, size_type c2) noexcept
{
    m_engine.swap_columns(c1, c2);
}

template<class ET, class OT>
template<class ET2, detail::enable_if_writable<ET, ET2>> inline constexpr
void
matrix<ET,OT>::swap_rows(size_type r1, size_type r2) noexcept
{
    m_engine.swap_rows(r1, r2);
}

//------------
//- Comparison
//
template<class ET1, class OT1, class ET2, class OT2>
constexpr bool
operator ==(matrix<ET1, OT1> const& lhs, matrix<ET2, OT2> const& rhs)
{
    using lhs_size = typename ET1::size_type;
    using rhs_size = typename ET2::size_type;

    lhs_size    i1 = 0;
    lhs_size    j1 = 0;
    lhs_size    r1 = lhs.rows();
    lhs_size    c1 = lhs.columns();

    rhs_size    i2 = 0;
    rhs_size    j2 = 0;
    rhs_size    r2 = rhs.rows();
    rhs_size    c2 = rhs.columns();

    if (r1 != static_cast<lhs_size>(r2)  ||  c1 != static_cast<lhs_size>(c2)) return false;

    for (;  i1 < r1;  ++i1, ++i2)
    {
        for (;  j1 < c1;  ++j1, ++j2)
        {
            if (lhs(i1, j1) != rhs(i2, j2)) return false;
        }
    }
    return true;
}

template<class ET1, class OT1, class ET2, class OT2>
constexpr bool
operator !=(matrix<ET1, OT1> const& lhs, matrix<ET2, OT2> const& rhs)
{
    return !(lhs == rhs);
}

#ifdef LA_USE_MDSPAN

template<class ET, class OT, class T, ptrdiff_t X0, ptrdiff_t X1, class L, class A> constexpr
bool
operator ==(matrix<ET, OT> const& lhs, basic_mdspan<T, extents<X0, X1>, L, A> const& rhs)
{
    using lhs_size = typename ET::size_type;
    using rhs_size = typename basic_mdspan<T, extents<X0, X1>, L, A>::index_type;

    lhs_size    i1 = 0;
    lhs_size    j1 = 0;
    lhs_size    r1 = lhs.rows();
    lhs_size    c1 = lhs.columns();

    rhs_size    i2 = 0;
    rhs_size    j2 = 0;
    rhs_size    r2 = rhs.extent(0);
    rhs_size    c2 = rhs.extent(1);

    if (r1 != static_cast<lhs_size>(r2)  ||  c1 != static_cast<lhs_size>(c2)) return false;

    for (;  i1 < r1;  ++i1, ++i2)
    {
        for (;  j1 < c1;  ++j1, ++j2)
        {
            if (lhs(i1, j1) != rhs(i2, j2)) return false;
        }
    }
    return true;
}

template<class ET, class OT, class T, ptrdiff_t X0, ptrdiff_t X1, class L, class A> constexpr
bool
operator !=(matrix<ET, OT> const& lhs, basic_mdspan<T, extents<X0, X1>, L, A> const& rhs)
{
    return !(lhs == rhs);
}

template<class T, ptrdiff_t X0, ptrdiff_t X1, class L, class A, class ET, class OT> constexpr
bool
operator ==(basic_mdspan<T, extents<X0, X1>, L, A> const& lhs, matrix<ET, OT> const& rhs)
{
    using lhs_size = typename basic_mdspan<T, extents<X0, X1>, L, A>::index_type;
    using rhs_size = typename ET::size_type;

    lhs_size    i1 = 0;
    lhs_size    j1 = 0;
    lhs_size    r1 = lhs.rows();
    lhs_size    c1 = lhs.columns();

    rhs_size    i2 = 0;
    rhs_size    j2 = 0;
    rhs_size    r2 = rhs.extent(0);
    rhs_size    c2 = rhs.extent(1);

    if (r2 != static_cast<lhs_size>(r1)  ||  c2 != static_cast<lhs_size>(c1)) return false;

    for (;  i1 < r1;  ++i1, ++i2)
    {
        for (;  j1 < c1;  ++j1, ++j2)
        {
            if (lhs(i1, j1) != rhs(i2, j2)) return false;
        }
    }
    return true;
}

template<class T, ptrdiff_t X0, ptrdiff_t X1, class L, class A, class ET, class OT> constexpr
bool
operator !=(basic_mdspan<T, extents<X0, X1>, L, A> const& lhs, matrix<ET, OT> const& rhs)
{
    return !(lhs == rhs);
}

#endif

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_MATRIX_HPP_DEFINED
