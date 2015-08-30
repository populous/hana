/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_LAWS_INTEGRAL_DOMAIN_HPP
#define BOOST_HANA_TEST_LAWS_INTEGRAL_DOMAIN_HPP

#include <boost/hana/assert.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/concept/comparable.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/concept/integral_domain.hpp>
#include <boost/hana/lazy.hpp>

#include <laws/base.hpp>


namespace boost { namespace hana { namespace test {
    template <typename D, typename = when<true>>
    struct TestIntegralDomain : TestIntegralDomain<D, laws> {
        using TestIntegralDomain<D, laws>::TestIntegralDomain;
    };

    template <typename D>
    struct TestIntegralDomain<D, laws> {
        template <typename Xs>
        TestIntegralDomain(Xs xs) {
            hana::for_each(xs, [](auto x) {
                static_assert(_models<IntegralDomain, decltype(x)>{}, "");
            });

            foreach2(xs, [](auto a, auto b) {

                // commutativity
                BOOST_HANA_CHECK(hana::equal(
                    hana::mult(a, b),
                    hana::mult(b, a)
                ));

                only_when_(hana::not_equal(b, zero<D>()),
                hana::make_lazy([](auto a, auto b) {
                    BOOST_HANA_CHECK(hana::equal(
                        hana::plus(
                            hana::mult(hana::quot(a, b), b),
                            hana::rem(a, b)
                        ),
                        a
                    ));

                    BOOST_HANA_CHECK(hana::equal(
                        hana::rem(zero<D>(), b),
                        zero<D>()
                    ));
                })(a, b));

            });
        }
    };

    template <typename C>
    struct TestIntegralDomain<C, when<_models<Constant, C>::value>>
        : TestIntegralDomain<C, laws>
    {
        template <typename Xs>
        TestIntegralDomain(Xs xs) : TestIntegralDomain<C, laws>{xs} {
            foreach2(xs, [](auto x, auto y) {
                only_when_(hana::not_equal(zero<C>(), y),
                hana::make_lazy([](auto x, auto y) {
                    BOOST_HANA_CHECK(hana::equal(
                        hana::quot(hana::value(x), hana::value(y)),
                        hana::value(hana::quot(x, y))
                    ));

                    BOOST_HANA_CHECK(hana::equal(
                        hana::rem(hana::value(x), hana::value(y)),
                        hana::value(hana::rem(x, y))
                    ));

                })(x, y));
            });
        }
    };
}}} // end namespace boost::hana::test

#endif // !BOOST_HANA_TEST_LAWS_INTEGRAL_DOMAIN_HPP
