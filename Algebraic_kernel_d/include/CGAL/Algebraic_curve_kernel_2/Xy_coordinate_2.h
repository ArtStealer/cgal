// TODO: Add licence
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL:$
// $Id: $
// 
//
// Author(s)     : Eric Berberich <eric@mpi-inf.mpg.de>
//                 Pavel Emeliyanenko <asm@mpi-sb.mpg.de>
//
// ============================================================================

#ifndef CGAL_ALGEBRAIC_CURVE_KERNEL_XY_COORDINATE_2_H
#define CGAL_ALGEBRAIC_CURVE_KERNEL_XY_COORDINATE_2_H

#include <CGAL/basic.h>
#include <boost/pool/pool_alloc.hpp>

CGAL_BEGIN_NAMESPACE
//::boost::fast_pool_allocator<Rep_> >
namespace CGALi {

template < class AlgebraicCurveKernel_2, class Rep_, 
      class HandlePolicy_ = CGAL::Handle_policy_no_union,
      class Allocator_ = CGAL_ALLOCATOR(Rep_) >
      
      //::boost::fast_pool_allocator<Rep_> >
class Xy_coordinate_2;

template < class AlgebraicCurveKernel_2, class Rep, class HandlePolicy,
           class Allocator > 
std::ostream& operator<< (std::ostream&, 
    const Xy_coordinate_2<AlgebraicCurveKernel_2, Rep, HandlePolicy,
        Allocator>&);

template < class AlgebraicCurveKernel_2 >
class Xy_coordinate_2_rep {

public:
    // this first template argument
    typedef AlgebraicCurveKernel_2 Algebraic_curve_kernel_2;

    // myself
    typedef Xy_coordinate_2_rep<Algebraic_curve_kernel_2> Self;

    typedef typename Algebraic_curve_kernel_2::Curve_pair_2 Curve_pair_2;

    typedef typename Curve_pair_2::Algebraic_curve_2 Curve_2; 
    
    typedef typename Curve_2::X_coordinate X_coordinate_1; 

    // constructors
public:
    // default constructor ()
    Xy_coordinate_2_rep() : _m_arcno(-1)
    {   }
    
    // standard constructor
    Xy_coordinate_2_rep(const X_coordinate_1& x, const Curve_2& curve, 
                int arcno) : _m_x(x), _m_curve(curve), _m_arcno(arcno)
    {   }

    // data
    // x-coordinate
    X_coordinate_1 _m_x;
    
    // supporting curve
    mutable Curve_2 _m_curve;
    
    // arc number on curve
    mutable int _m_arcno;

    // befriending the handle
    friend class Xy_coordinate_2<Algebraic_curve_kernel_2, Self>;
};

//! \brief class \c Xy_coordinate_2 represents a single root of a system of 
//! two polynomial equations in two variables that are models 
//! \c AlgebraicCurveKernel_2::Polynomial_2
//!
//! \c Xy_coordinate_2 coordinate is represented by an \c X_coordinate_1,
//! a supporting curve and an arcno and is valid only for finite solutions,
//! i.e., it cannot represent points at infinity 
template <class AlgebraicCurveKernel_2, 
          class Rep_ = CGALi::Xy_coordinate_2_rep<AlgebraicCurveKernel_2>,
          class HandlePolicy_, class Allocator_>
class Xy_coordinate_2 : 
    public ::CGAL::Handle_with_policy<Rep_, HandlePolicy_, Allocator_> 
{
public:
    //! \name public typedefs
    //!@{
    
    //! this instance's first template parameter
    typedef AlgebraicCurveKernel_2 Algebraic_curve_kernel_2;

    //! this instance's second template parameter
    typedef Rep_ Rep;
    
    //! this instance's third template parameter
    typedef HandlePolicy_ Handle_policy;
    
    //! this instance's fourth template parameter
    typedef Allocator_ Allocator;

    //! this instance itself
    typedef Xy_coordinate_2<Algebraic_curve_kernel_2, Rep, Handle_policy,
        Allocator> Self;
    
    //! type of a curve pair 
    typedef typename Algebraic_curve_kernel_2::Curve_pair_2 Curve_pair_2;

    //! type of an algabraic curve
    typedef typename Curve_pair_2::Algebraic_curve_2 Curve_2; 

    //! type of X_coordinate
    typedef typename Curve_2::X_coordinate X_coordinate_1;

    //! type of curve pair analysis
    typedef typename Algebraic_curve_kernel_2::Curve_pair_analysis_2
                Curve_pair_analysis_2;
    
    //! type of pair vertical line
    typedef typename Curve_pair_analysis_2::Curve_pair_vertical_line_1
                Curve_pair_vertical_line_1;

    //! type of curve analysis
    typedef typename Algebraic_curve_kernel_2::Curve_analysis_2
                Curve_analysis_2;
    
    //! type of vertical line
    typedef typename Curve_analysis_2::Curve_vertical_line_1
                Curve_vertical_line_1;
    
    //! the handle superclass
    typedef ::CGAL::Handle_with_policy<Rep, Handle_policy, Allocator> Base;

    typedef typename Algebraic_curve_kernel_2::X_real_traits_1 X_real_traits_1;

    //! Algebraic_real_traits for this type
    typedef typename Algebraic_curve_kernel_2::Y_real_traits_1 Y_real_traits_1;

    //! type for approximation boundaries
    typedef typename X_real_traits_1::Boundary Boundary;

    //! type for boundary intervals
    typedef boost::numeric::interval<Boundary> Boundary_interval;
    


    //!@}
private:
    //! \name private methods
    //!@{

    /*!\brief
     * Simplifies the representation of two points whose supporting curves
     * share a common part.
     */
    static bool _simplify(const Xy_coordinate_2& p, const Xy_coordinate_2& q) 
    {
        std::vector<Curve_2> parts_of_f, parts_of_g, common;
        Algebraic_curve_kernel_2 ak_2;

        if(ak_2.decompose_2_object()(p.curve(), q.curve(), 
            std::back_inserter(parts_of_f), std::back_inserter(parts_of_g),
                std::back_inserter(common))) {
            CGAL_assertion((parts_of_f.size() == 1 ||
                       parts_of_g.size() == 1) && common.size() == 1);
            if(parts_of_f.size() == 1) {
                p.simplify_by(Curve_pair_analysis_2(
                    (Curve_analysis_2(parts_of_f[0])),
                        (Curve_analysis_2(common[0]))));
            } 
            if(parts_of_g.size() == 1) {
                q.simplify_by(Curve_pair_analysis_2(
                    (Curve_analysis_2(parts_of_g[0])),
                        (Curve_analysis_2(common[0]))));
            } 
            return true;
        }
        return false;
    }
    
    //!@}
public:
    //!\name Constructors
    //!@{

    /*!\brief 
     * default constructor
     *
     * A default-constructed point supports no operation other than
     * having \c curve().degree() return \c -1. 
     */
    Xy_coordinate_2() : 
        Base(Rep()) { 
    }

    /*!\brief
     * copy constructor
     */
    Xy_coordinate_2(const Self& p) : 
        Base(static_cast<const Base&>(p)) {  
    }

    /*!\brief
     * Point at \c x, on \c curve with \c arcno. Finite points on vertical arcs
     * are also constructed in this way
     */
    Xy_coordinate_2(const X_coordinate_1& x, const Curve_2& curve, int arcno) :
            Base(Rep(x, curve, arcno)) {
            
        CGAL_precondition(arcno >= 0);
        CGAL_precondition_code(
            Curve_analysis_2 ca(curve);
            typename Curve_analysis_2::Curve_vertical_line_1 v = 
                ca.vertical_line_for_x(x);
        );
        CGAL_precondition(arcno >= 0 && arcno < v.number_of_events());
    }
    
    /*!\brief
     * constructs a point from a given represenation
     */
    Xy_coordinate_2(Rep rep) : 
        Base(rep) {  
    }
   
    //!@}
public:
    //!\name Access functions
    //!@{
    
    /*!\brief 
     * x-coordinate of the point
     */
    X_coordinate_1 x() const { 
        return this->ptr()->_m_x; 
    }

    /*!
     * \brief y-coordinate of this point
     *
     * TODO: Loos-algorithm
     */
    X_coordinate_1 y() const {
        CGAL_error("Not yet implemented");
        return this->ptr()->_m_x; 
    }
    
    /*!\brief
     * supporting curve of the point
     */
    Curve_2 curve() const { 
        return this->ptr()->_m_curve; 
    }
    
    /*!\brief
     * arc number of point
     *
     */
    int arcno() const { 
        return this->ptr()->_m_arcno; 
    }

    //!@}
public:
    //!\name comparison predicates
    //!@{

    /*!\brief
     * compares x-coordinates of \c *this with \c q
     * 
     * do we need this method or one should use Algebraic_curve_kernel_2
     * directly ?
     */
    CGAL::Comparison_result compare_x(const Self& q) const {
    
        if(this->is_identical(q)) {
            return CGAL::EQUAL;
        }
        Algebraic_curve_kernel_2 ak_2;
        return ak_2.compare_x_2_object()(this->x(), q.x());
    }

    /*!\brief
     * compares \c *this with \c q lexicographically
     */
    CGAL::Comparison_result compare_xy(const Self& q, 
        bool equal_x = false) const {
        
        if(is_identical(q)) 
            return CGAL::EQUAL;
        if(!equal_x) {
            CGAL::Comparison_result c = compare_x(q);
            if(c != CGAL::EQUAL) 
                return c;
        }
        return _compare_y_at_x(q);
    }
    
    //! equality
    bool operator == (const Self& q) const {return q.compare_xy(*this)== 0;}
    
    //! inequality
    bool operator != (const Self& q) const {return q.compare_xy(*this)!= 0;}

    //! less than in (x,y) lexicographic order
    bool operator <  (const Self& q) const {return q.compare_xy(*this)> 0;}

    //! less-equal in (x,y) lexicographic order
    bool operator <= (const Self& q) const {return q.compare_xy(*this)>= 0;}

    //! greater than in (x,y) lexicographic order
    bool operator >  (const Self& q) const {return q.compare_xy(*this)< 0;}

    //! greater-equal in (x,y) lexicographic order
    bool operator >= (const Self& q) const {return q.compare_xy(*this)<= 0;}
    
    //!@}
private:
    //!@{
    //! \name 

    /*!\brief
     * compares y-coordinates for covertical points \c *this and \c q
     *
     * \pre x() == q.x()
     */
    CGAL::Comparison_result _compare_y_at_x(const Self& q) const 
    {
        CGAL_precondition(this->compare_x(q) == CGAL::EQUAL);
    
        Curve_2 f = this->curve();
        Curve_2 g = q.curve();
        // may have common factor ?
        if(!f.is_identical(g) && Self::_simplify(*this,q)) {
            // restart because supporting curves might be equal now
            return this->_compare_y_at_x(q);
        } 
        if(f.is_identical(g)) 
            return CGAL::sign(this->arcno() - q.arcno());
        // this is to keep compiler happy ))
        Curve_pair_analysis_2 cpa_2((Curve_analysis_2(f)),
             (Curve_analysis_2(g)));
        const Curve_pair_vertical_line_1& vline = 
            cpa_2.vertical_line_for_x(x());
            
        return CGAL::sign(vline.get_event_of_curve(this->arcno(), 0) - 
                    vline.get_event_of_curve(q.arcno(), 1));
    }
    
    //!@}
public:
    //!\name Reconstructing functions
    //!@{
    
    /*!\brief
     * Simplifies the representation of a point.
     * 
     * Given a decomposition of the point's supporting \c curve() into 
     * a pair of two curves \c pair, this function searches this point
     * in the curve pair and resets the curve and the arcno to this
     * found arc. It can happen, that both curves of the pair fit this 
     * condition (intersection of the two curves at this point), then it
     * chooses the simpler one (less total degree).
     *
     * \pre pair must be a decomposition of curve()
     */
    void simplify_by(const Curve_pair_analysis_2& cpa_2) const 
    { 
        typedef typename Algebraic_curve_kernel_2::Polynomial_2_CGAL Poly_2;
        typename Algebraic_curve_kernel_2::NiX2CGAL_converter cvt;
        typedef typename CGAL::Polynomial_traits_d<Poly_2>::Total_degree
            Total_degree;
        
        CGAL_precondition_code(
            typename Curve_2::Poly_d mult =
                    cpa_2.get_curve_analysis(0).get_polynomial_2().f() *
                    cpa_2.get_curve_analysis(1).get_polynomial_2().f();
            Total_degree total_degree;
        );
        // common parts
        CGAL_precondition(NiX::resultant(mult, curve().f()).is_zero());
        // full parts
        CGAL_precondition(mult.degree() == curve().f().degree());
        CGAL_precondition(total_degree(cvt(mult)) ==
            total_degree(cvt(curve().f())));

        const Curve_pair_vertical_line_1& cpv_line =
                cpa_2.vertical_line_for_x(x());
        // # of arcs must match
        CGAL_precondition_code(
            const Curve_vertical_line_1& cv_line = 
                Curve_analysis_2(curve()).vertical_line_for_x(x());
        );
        CGAL_precondition(cpv_line.number_of_events() == 
            cv_line.number_of_events());

        int cid = 0;
        std::pair<int, int> p = cpv_line.get_curves_at_event(arcno());
        if(p.first != -1 && p.second != -1) {
            // both curves involved: choose simpler one
            // Remark: In this case, a vertical line in the curves can be
            // ignored, since it has not been considered when constructing
            // the point from the composed curved (also including this vertical
            // line). Therefore, the old arc number is also valid in the curve
            // pair.
            Total_degree total_degree;
            Poly_2 ff=cvt(cpa_2.get_curve_analysis(0).get_polynomial_2().f()),
                   gg=cvt(cpa_2.get_curve_analysis(1).get_polynomial_2().f());
            if(total_degree(ff) > total_degree(gg)) 
                cid = 1;
        } else 
            cid = (p.first != -1 ? 0 : 1);
        // overwrite data
        this->ptr()->_m_curve =
            cpa_2.get_curve_analysis(cid).get_polynomial_2();
        this->ptr()->_m_arcno = (cid == 0 ? p.first : p.second);
    }
    
    //! befriending output iterator
    friend std::ostream& operator << <>(std::ostream& os, const Self& pt);

    //!@}

    public:
    //!\name Approximating functions
    //!@{

    /*!
     * \brief gets approximation of x
     *
     * TODO: give a interval width as argument?
     */
    Boundary_interval get_approximation_x() const {
        
        typename X_real_traits_1::Lower_boundary lower;
        typename X_real_traits_1::Upper_boundary upper;

        return Boundary_interval(lower(this->ptr()->_m_x), 
                                 upper(this->ptr()->_m_x));
        
    }

    /*!
     * \brief gets approximation of y
     *
     * TODO: give a interval width as argument?
     */
    Boundary_interval get_approximation_y() const {
        typename Y_real_traits_1::Lower_boundary lower;
        typename Y_real_traits_1::Upper_boundary upper;
        return Boundary_interval(lower(*this), upper(*this));
    }
    
    /*!
     * \brief Refines the x-xoordinate
     */
    void refine_x() const {
        typename X_real_traits_1::Refine refine;
        refine(this->ptr()->_m_x);
    }

    /*!
     * \brief Refines the x-xoordinate
     */
    void refine_y() const {
        typename Y_real_traits_1::Refine refine;
        refine(*this);
    }
    

    //!@}

}; // class Xy_coordinate_2

template < class AlgebraicCurveKernel_2, class Rep, class HandlePolicy,
           class Allocator > 
std::ostream& operator<< (std::ostream& os, 
    const Xy_coordinate_2<AlgebraicCurveKernel_2, Rep, HandlePolicy,
        Allocator>& pt)
{
    if(::CGAL::get_mode(os) == ::CGAL::IO::PRETTY) {
        os << "[x-coord: " << NiX::to_double(pt.x()) << "; curve: " <<
            pt.curve().f() << 
            "; arcno: " << pt.arcno() << "]\n";
    } else { // ASCII output
        os << "[x-coord: " << NiX::to_double(pt.x()) << "; curve: " <<
            pt.curve().f() << 
            "; arcno: " << pt.arcno() << "]\n";
    }
    return os;    
}

} // namespace CGALi

CGAL_END_NAMESPACE

#endif // CGAL_ALGEBRAIC_CURVE_KERNEL_XY_COORDINATE_2_H
