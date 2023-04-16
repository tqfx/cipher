/*!
 @file polytrack.h
 @brief polynomial trajectory
 @details Trajectory Planning for Automatic Machines and Robots
*/

#ifndef LIBA_POLYTRACK_H
#define LIBA_POLYTRACK_H

#include "a.h"

/*!
 @ingroup A
 @addtogroup A_POLYTRACK polynomial trajectory
 @{
*/

/*!
 @brief instance structure for cubic polynomial trajectory
*/
typedef struct a_polytrack3_s
{
    a_real_t k[4]; //!< quantity
    a_real_t t[2]; //!< time unit(s)
    a_real_t q[2]; //!< position
    a_real_t v[2]; //!< velocity
} a_polytrack3_s;

/*!
 @brief instance structure for quintic polynomial trajectory
*/
typedef struct a_polytrack5_s
{
    a_real_t k[6]; //!< quantity
    a_real_t t[2]; //!< time unit(s)
    a_real_t q[2]; //!< position
    a_real_t v[2]; //!< velocity
    a_real_t a[2]; //!< acceleration
} a_polytrack5_s;

/*!
 @brief instance structure for hepta polynomial trajectory
*/
typedef struct a_polytrack7_s
{
    a_real_t k[8]; //!< quantity
    a_real_t t[2]; //!< time unit(s)
    a_real_t q[2]; //!< position
    a_real_t v[2]; //!< velocity
    a_real_t a[2]; //!< acceleration
    a_real_t j[2]; //!< jerk
} a_polytrack7_s;

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/* function for cubic polynomial trajectory */

/*!
 @brief initialize function for cubic polynomial trajectory
 @param[in,out] ctx points to an instance of cubic polynomial trajectory
 @param[in] t time
  @arg 0 source
  @arg 1 target
 @param[in] q position
  @arg 0 source
  @arg 1 target
 @param[in] v velocity
  @arg 0 source
  @arg 1 target
*/
A_EXTERN a_void_t a_polytrack3_init1(a_polytrack3_s *ctx, a_real_t const t[2], a_real_t const q[2], a_real_t const v[2]);

/*!
 @brief initialize function for cubic polynomial trajectory
 @param[in,out] ctx points to an instance of cubic polynomial trajectory
 @param[in] source source for trajectory
  @arg 0 time for source
  @arg 1 position for source
  @arg 2 velocity for source
 @param[in] target target for trajectory
  @arg 0 time for target
  @arg 1 position for target
  @arg 2 velocity for target
*/
A_EXTERN a_void_t a_polytrack3_init2(a_polytrack3_s *ctx, a_real_t const source[3], a_real_t const target[3]);

/*!
 @brief initialize function for cubic polynomial trajectory
 @param[in,out] ctx points to an instance of cubic polynomial trajectory
 @param[in] t0 time for source
 @param[in] t1 time for target
 @param[in] q0 position for source
 @param[in] q1 position for target
 @param[in] v0 velocity for source
 @param[in] v1 velocity for target
*/
A_EXTERN a_void_t a_polytrack3_init(a_polytrack3_s *ctx,
                                    a_real_t t0, a_real_t t1,
                                    a_real_t q0, a_real_t q1,
                                    a_real_t v0, a_real_t v1);

/*!
 @brief generation function for cubic polynomial trajectory
 @f{aligned}{
  \left\{\begin{array}{l}
  t=t_{1}-t_{0}\\
  q=q_{1}-q_{0}\\
  k_{0}=q_{0}\\
  k_{1}=v_{0}\\
  k_{2}=\cfrac{\left(-2\,v_{0}-v_{1}\right)\,t+3\,q}{t^2}\\
  k_{3}=\cfrac{\left(v_{0}+v_{1}\right)\,t-2\,q}{t^3}
  \end{array}\right.
 @f}
 @param[in,out] ctx points to an instance of cubic polynomial trajectory
*/
A_EXTERN a_void_t a_polytrack3_gen(a_polytrack3_s *ctx);

/*!
 @brief process function for cubic polynomial trajectory
 @f{aligned}{
  \begin{array}{l}
  q(t)=k_{0}+k_{1}\left(t-t_{0}\right)+k_{2}\left(t-t_{0}\right)^{2}+k_{3}\left(t-t_{0}\right)^{3} \\
  \dot{q}(t)=k_{1}+2 k_{2}\left(t-t_{0}\right)+3 k_{3}\left(t-t_{0}\right)^{2} \\
  \ddot{q}(t)=2 k_{2}+6 k_{3}\left(t-t_{0}\right)
  \end{array}
 @f}
 @param[in] ctx points to an instance of cubic polynomial trajectory
 @param[in] ts current time unit(s)
 @param[out] out buffer for result
  @arg 0 position output
  @arg 1 velocity output
  @arg 2 acceleration output
*/
A_EXTERN a_void_t a_polytrack3_out(a_polytrack3_s const *ctx, a_real_t ts, a_real_t out[3]);

/*!
 @brief process function for cubic polynomial trajectory position
 @f{aligned}{
  \begin{array}{l}
  q(t)=k_{0}+k_{1}\left(t-t_{0}\right)+k_{2}\left(t-t_{0}\right)^{2}+k_{3}\left(t-t_{0}\right)^{3} \\
  \end{array}
 @f}
 @param[in] ctx points to an instance of cubic polynomial trajectory
 @param[in] ts current time unit(s)
 @return position output
*/
A_EXTERN a_real_t a_polytrack3_pos(a_polytrack3_s const *ctx, a_real_t ts);

/*!
 @brief process function for cubic polynomial trajectory velocity
 @f{aligned}{
  \begin{array}{l}
  \dot{q}(t)=k_{1}+2 k_{2}\left(t-t_{0}\right)+3 k_{3}\left(t-t_{0}\right)^{2} \\
  \end{array}
 @f}
 @param[in] ctx points to an instance of cubic polynomial trajectory
 @param[in] ts current time unit(s)
 @return velocity output
*/
A_EXTERN a_real_t a_polytrack3_vec(a_polytrack3_s const *ctx, a_real_t ts);

/*!
 @brief process function for cubic polynomial trajectory acceleration
 @f{aligned}{
  \begin{array}{l}
  \ddot{q}(t)=2 k_{2}+6 k_{3}\left(t-t_{0}\right)
  \end{array}
 @f}
 @param[in] ctx points to an instance of cubic polynomial trajectory
 @param[in] ts current time unit(s)
 @return acceleration output
*/
A_EXTERN a_real_t a_polytrack3_acc(a_polytrack3_s const *ctx, a_real_t ts);

/* function for quintic polynomial trajectory */

/*!
 @brief initialize function for quintic polynomial trajectory
 @param[in,out] ctx points to an instance of quintic polynomial trajectory
 @param[in] t time
  @arg 0 source
  @arg 1 target
 @param[in] q position
  @arg 0 source
  @arg 1 target
 @param[in] v velocity
  @arg 0 source
  @arg 1 target
 @param[in] a acceleration
  @arg 0 source
  @arg 1 target
*/
A_EXTERN a_void_t a_polytrack5_init1(a_polytrack5_s *ctx, a_real_t const t[2], a_real_t const q[2], a_real_t const v[2], a_real_t const a[2]);

/*!
 @brief initialize function for quintic polynomial trajectory
 @param[in,out] ctx points to an instance of quintic polynomial trajectory
 @param[in] source source for trajectory
  @arg 0 time for source
  @arg 1 position for source
  @arg 2 velocity for source
  @arg 3 acceleration for source
 @param[in] target target for trajectory
  @arg 0 time for target
  @arg 1 position for target
  @arg 2 velocity for target
  @arg 3 acceleration for target
*/
A_EXTERN a_void_t a_polytrack5_init2(a_polytrack5_s *ctx, a_real_t const source[4], a_real_t const target[4]);

/*!
 @brief initialize function for quintic polynomial trajectory
 @param[in,out] ctx points to an instance of quintic polynomial trajectory
 @param[in] t0 time for source
 @param[in] t1 time for target
 @param[in] q0 position for source
 @param[in] q1 position for target
 @param[in] v0 velocity for source
 @param[in] v1 velocity for target
 @param[in] a0 acceleration for source
 @param[in] a1 acceleration for target
*/
A_EXTERN a_void_t a_polytrack5_init(a_polytrack5_s *ctx,
                                    a_real_t t0, a_real_t t1,
                                    a_real_t q0, a_real_t q1,
                                    a_real_t v0, a_real_t v1,
                                    a_real_t a0, a_real_t a1);

/*!
 @brief generation function for quintic polynomial trajectory
 @f{aligned}{
  \left\{\begin{array}{l}
  t=t_{1}-t_{0}\\
  q=q_{1}-q_{0}\\
  k_{0}=q_{0}\\
  k_{1}=v_{0}\\
  k_{2}=\cfrac{a_{0}}{2}\\
  k_{3}=\cfrac{\left(a_{1}-3\,a_{0}\right)\,t^2+\left(-12\,v_{0}-8\,v_{1}\right)\,t+20\,q}{2\,t^3}\\
  k_{4}=\cfrac{\left(3\,a_{0}-2\,a_{1}\right)\,t^2+\left(16\,v_{0}+14\,v_{1}\right)\,t-30\,q}{2\,t^4}\\
  k_{5}=\cfrac{\left(a_{1}-a_{0}\right)\,t^2+\left(-6\,v_{0}-6\,v_{1}\right)\,t+12\,q}{2\,t^5}
  \end{array}\right.
 @f}
 @param[in,out] ctx points to an instance of quintic polynomial trajectory
*/
A_EXTERN a_void_t a_polytrack5_gen(a_polytrack5_s *ctx);

/*!
 @brief process function for quintic polynomial trajectory
 @f{aligned}{
  \begin{array}{l}
  q(t)=k_{0}+k_{1}\left(t-t_{0}\right)+k_{2}\left(t-t_{0}\right)^{2}+k_{3}\left(t-t_{0}\right)^{3}+k_{4}\left(t-t_{0}\right)^{4}+k_{5}\left(t-t_{0}\right)^{5}\\
  \dot{q}(t)=k_{1}+2 k_{2}\left(t-t_{0}\right)+3 k_{3}\left(t-t_{0}\right)^{2}+4 k_{4}\left(t-t_{0}\right)^{3}+5 k_{5}\left(t-t_{0}\right)^{4}\\
  \ddot{q}(t)=2 k_{2}+6 k_{3}\left(t-t_{0}\right)+12 k_{4}\left(t-t_{0}\right)^{2}+20 k_{5}\left(t-t_{0}\right)^{3}
  \end{array}
 @f}
 @param[in] ctx points to an instance of quintic polynomial trajectory
 @param[in] ts current time unit(s)
 @param[out] out buffer for result
  @arg 0 position output
  @arg 1 velocity output
  @arg 2 acceleration output
*/
A_EXTERN a_void_t a_polytrack5_out(a_polytrack5_s const *ctx, a_real_t ts, a_real_t out[3]);

/*!
 @brief process function for quintic polynomial trajectory position
 @f{aligned}{
  \begin{array}{l}
  q(t)=k_{0}+k_{1}\left(t-t_{0}\right)+k_{2}\left(t-t_{0}\right)^{2}+k_{3}\left(t-t_{0}\right)^{3}+k_{4}\left(t-t_{0}\right)^{4}+k_{5}\left(t-t_{0}\right)^{5}\\
  \end{array}
 @f}
 @param[in] ctx points to an instance of quintic polynomial trajectory
 @param[in] ts current time unit(s)
 @return position output
*/
A_EXTERN a_real_t a_polytrack5_pos(a_polytrack5_s const *ctx, a_real_t ts);

/*!
 @brief process function for quintic polynomial trajectory velocity
 @f{aligned}{
  \begin{array}{l}
  \dot{q}(t)=k_{1}+2 k_{2}\left(t-t_{0}\right)+3 k_{3}\left(t-t_{0}\right)^{2}+4 k_{4}\left(t-t_{0}\right)^{3}+5 k_{5}\left(t-t_{0}\right)^{4}\\
  \end{array}
 @f}
 @param[in] ctx points to an instance of quintic polynomial trajectory
 @param[in] ts current time unit(s)
 @return velocity output
*/
A_EXTERN a_real_t a_polytrack5_vec(a_polytrack5_s const *ctx, a_real_t ts);

/*!
 @brief process function for quintic polynomial trajectory acceleration
 @f{aligned}{
  \begin{array}{l}
  \ddot{q}(t)=2 k_{2}+6 k_{3}\left(t-t_{0}\right)+12 k_{4}\left(t-t_{0}\right)^{2}+20 k_{5}\left(t-t_{0}\right)^{3}
  \end{array}
 @f}
 @param[in] ctx points to an instance of quintic polynomial trajectory
 @param[in] ts current time unit(s)
 @return acceleration output
*/
A_EXTERN a_real_t a_polytrack5_acc(a_polytrack5_s const *ctx, a_real_t ts);

/* function for hepta polynomial trajectory */

/*!
 @brief initialize function for hepta polynomial trajectory
 @param[in,out] ctx points to an instance of hepta polynomial trajectory
 @param[in] t time
  @arg 0 source
  @arg 1 target
 @param[in] q position
  @arg 0 source
  @arg 1 target
 @param[in] v velocity
  @arg 0 source
  @arg 1 target
 @param[in] a acceleration
  @arg 0 source
  @arg 1 target
 @param[in] j jerk
  @arg 0 source
  @arg 1 target
*/
A_EXTERN a_void_t a_polytrack7_init1(a_polytrack7_s *ctx, a_real_t const t[2], a_real_t const q[2], a_real_t const v[2], a_real_t const a[2], a_real_t const j[2]);

/*!
 @brief initialize function for hepta polynomial trajectory
 @param[in,out] ctx points to an instance of hepta polynomial trajectory
 @param[in] source source for trajectory
  @arg 0 time for source
  @arg 1 position for source
  @arg 2 velocity for source
  @arg 3 acceleration for source
  @arg 4 jerk for source
 @param[in] target target for trajectory
  @arg 0 time for target
  @arg 1 position for target
  @arg 2 velocity for target
  @arg 3 acceleration for target
  @arg 4 jerk for target
*/
A_EXTERN a_void_t a_polytrack7_init2(a_polytrack7_s *ctx, a_real_t const source[5], a_real_t const target[5]);

/*!
 @brief initialize function for hepta polynomial trajectory
 @param[in,out] ctx points to an instance of hepta polynomial trajectory
 @param[in] t0 time for source
 @param[in] t1 time for target
 @param[in] q0 position for source
 @param[in] q1 position for target
 @param[in] v0 velocity for source
 @param[in] v1 velocity for target
 @param[in] a0 acceleration for source
 @param[in] a1 acceleration for target
 @param[in] j0 jerk for source
 @param[in] j1 jerk for target
*/
A_EXTERN a_void_t a_polytrack7_init(a_polytrack7_s *ctx,
                                    a_real_t t0, a_real_t t1,
                                    a_real_t q0, a_real_t q1,
                                    a_real_t v0, a_real_t v1,
                                    a_real_t a0, a_real_t a1,
                                    a_real_t j0, a_real_t j1);

/*!
 @brief generation function for hepta polynomial trajectory
 @f{aligned}{
  \left\{\begin{array}{l}
  t=t_{1}-t_{0}\\
  q=q_{1}-q_{0}\\
  k_{0}=q_{0}\\
  k_{1}=v_{0}\\
  k_{2}=\cfrac{a_{0}}{2}\\
  k_{3}=\cfrac{j_{0}}{6}\\
  k_{4}=\cfrac{\left(-4\,j_{0}-j_{1}\right)\,t^3+\left(15\,a_{1}-30\,a_{0}\right)\,t^2+\left(-120\,v_{0}-90\,v_{1}\right)\,t+210\,q}{6\,t^4}\\
  k_{5}=\cfrac{\left(2\,j_{0}+j_{1}\right)\,t^3+\left(20\,a_{0}-14\,a_{1}\right)\,t^2+\left(90\,v_{0}+78\,v_{1}\right)\,t-168\,q}{2\,t^5}\\
  k_{6}=\cfrac{\left(-4\,j_{0}-3\,j_{1}\right)\,t^3+\left(39\,a_{1}-45\,a_{0}\right)\,t^2+\left(-216\,v_{0}-204\,v_{1}\right)\,t+420\,q}{6\,t^6}\\
  k_{7}=\cfrac{\left(j_{0}+j_{1}\right)\,t^3+\left(12\,a_{0}-12\,a_{1}\right)\,t^2+\left(60\,v_{0}+60\,v_{1}\right)\,t-120\,q}{6\,t^7}
  \end{array}\right.
 @f}
 @param[in,out] ctx points to an instance of hepta polynomial trajectory
*/
A_EXTERN a_void_t a_polytrack7_gen(a_polytrack7_s *ctx);

/*!
 @brief process function for hepta polynomial trajectory
 @f{aligned}{
  \begin{array}{l}
  q(t)=k_{0}+k_{1}\left(t-t_{0}\right)+k_{2}\left(t-t_{0}\right)^{2}+k_{3}\left(t-t_{0}\right)^{3}+k_{4}\left(t-t_{0}\right)^{4}+k_{5}\left(t-t_{0}\right)^{5}+k_{6}\left(t-t_{0}\right)^{6}+k_{7}\left(t-t_{0}\right)^{7}\\
  \dot{q}(t)=k_{1}+2 k_{2}\left(t-t_{0}\right)+3 k_{3}\left(t-t_{0}\right)^{2}+4 k_{4}\left(t-t_{0}\right)^{3}+5 k_{5}\left(t-t_{0}\right)^{4}+6 k_{6}\left(t-t_{0}\right)^{5}+7 k_{7}\left(t-t_{0}\right)^{6}\\
  \ddot{q}(t)=2 k_{2}+6 k_{3}\left(t-t_{0}\right)+12 k_{4}\left(t-t_{0}\right)^{2}+20 k_{5}\left(t-t_{0}\right)^{3}+30 k_{6}\left(t-t_{0}\right)^{4}+42 k_{7}\left(t-t_{0}\right)^{5}\\
  q^{(3)}(t)=6 k_{3}+24 k_{4}\left(t-t_{0}\right)+60 k_{5}\left(t-t_{0}\right)^{2}+120 k_{6}\left(t-t_{0}\right)^{3}+210 k_{7}\left(t-t_{0}\right)^{4}
  \end{array}
 @f}
 @param[in] ctx points to an instance of hepta polynomial trajectory
 @param[in] ts current time unit(s)
 @param[out] out buffer for result
  @arg 0 position output
  @arg 1 velocity output
  @arg 2 acceleration output
  @arg 3 jerk output
*/
A_EXTERN a_void_t a_polytrack7_out(a_polytrack7_s const *ctx, a_real_t ts, a_real_t out[4]);

/*!
 @brief process function for hepta polynomial trajectory position
 @f{aligned}{
  \begin{array}{l}
  q(t)=k_{0}+k_{1}\left(t-t_{0}\right)+k_{2}\left(t-t_{0}\right)^{2}+k_{3}\left(t-t_{0}\right)^{3}+k_{4}\left(t-t_{0}\right)^{4}+k_{5}\left(t-t_{0}\right)^{5}+k_{6}\left(t-t_{0}\right)^{6}+k_{7}\left(t-t_{0}\right)^{7}\\
  \end{array}
 @f}
 @param[in] ctx points to an instance of hepta polynomial trajectory
 @param[in] ts current time unit(s)
 @return position output
*/
A_EXTERN a_real_t a_polytrack7_pos(a_polytrack7_s const *ctx, a_real_t ts);

/*!
 @brief process function for hepta polynomial trajectory velocity
 @f{aligned}{
  \begin{array}{l}
  \dot{q}(t)=k_{1}+2 k_{2}\left(t-t_{0}\right)+3 k_{3}\left(t-t_{0}\right)^{2}+4 k_{4}\left(t-t_{0}\right)^{3}+5 k_{5}\left(t-t_{0}\right)^{4}+6 k_{6}\left(t-t_{0}\right)^{5}+7 k_{7}\left(t-t_{0}\right)^{6}\\
  \end{array}
 @f}
 @param[in] ctx points to an instance of hepta polynomial trajectory
 @param[in] ts current time unit(s)
 @return velocity output
*/
A_EXTERN a_real_t a_polytrack7_vec(a_polytrack7_s const *ctx, a_real_t ts);

/*!
 @brief process function for hepta polynomial trajectory acceleration
 @f{aligned}{
  \begin{array}{l}
  \ddot{q}(t)=2 k_{2}+6 k_{3}\left(t-t_{0}\right)+12 k_{4}\left(t-t_{0}\right)^{2}+20 k_{5}\left(t-t_{0}\right)^{3}+30 k_{6}\left(t-t_{0}\right)^{4}+42 k_{7}\left(t-t_{0}\right)^{5}\\
  \end{array}
 @f}
 @param[in] ctx points to an instance of hepta polynomial trajectory
 @param[in] ts current time unit(s)
 @return acceleration output
*/
A_EXTERN a_real_t a_polytrack7_acc(a_polytrack7_s const *ctx, a_real_t ts);

/*!
 @brief process function for hepta polynomial trajectory jerk
 @f{aligned}{
  \begin{array}{l}
  q^{(3)}(t)=6 k_{3}+24 k_{4}\left(t-t_{0}\right)+60 k_{5}\left(t-t_{0}\right)^{2}+120 k_{6}\left(t-t_{0}\right)^{3}+210 k_{7}\left(t-t_{0}\right)^{4}
  \end{array}
 @f}
 @param[in] ctx points to an instance of hepta polynomial trajectory
 @param[in] ts current time unit(s)
 @return jerk output
*/
A_EXTERN a_real_t a_polytrack7_jer(a_polytrack7_s const *ctx, a_real_t ts);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* __cplusplus */

/*! @} A_POLYTRACK */

#endif /* polytrack.h */
