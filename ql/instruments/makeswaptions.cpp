/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2007 Giorgio Facchinetti

 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software: you can redistribute it and/or modify it
 under the terms of the QuantLib license.  You should have received a
 copy of the license along with this program; if not, please email
 <quantlib-dev@lists.sf.net>. The license is also available online at
 <http://quantlib.org/license.shtml>.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

#include <ql/instruments/makeswaptions.hpp>
#include <ql/cashflows/cashflows.hpp>
#include <ql/indexes/swapindex.hpp>
#include <ql/exercise.hpp>

namespace QuantLib {

    MakeSwaption::MakeSwaption(const boost::shared_ptr<SwapIndex>& swapIndex,
                             Rate strike,
                             const Period& optionTenor,
                             const boost::shared_ptr<PricingEngine>& engine,
                             Settlement::Type delivery)
    : delivery_(delivery),
      strike_(strike),
      optionTenor_(optionTenor),
      swapIndex_(swapIndex),
      swaptionConvention_(Following),
      engine_(engine) {
        
        Date evaluationDate = Settings::instance().evaluationDate();
        Date optionDate = swapIndex_->fixingCalendar().advance(evaluationDate,
                                                               optionTenor_, 
                                                               swaptionConvention_);
        exercise_ = boost::shared_ptr<Exercise>(new EuropeanExercise(optionDate));
        underlyingSwap_ = swapIndex->underlyingSwap(optionDate);

        if (strike_ == Null<Rate>())
            strike_ = CashFlows::atmRate(underlyingSwap_->floatingLeg(), 
                                         underlyingSwap_->termStructure());
    }

    MakeSwaption::operator Swaption() const {
        return Swaption(underlyingSwap_, exercise_, underlyingSwap_->termStructure(), engine_, delivery_);
    }

    MakeSwaption::operator boost::shared_ptr<Swaption>() const {
        return boost::shared_ptr<Swaption>(new
            Swaption(underlyingSwap_, exercise_, underlyingSwap_->termStructure(), engine_, delivery_));
    }

    MakeSwaption& MakeSwaption::withSwaptionConvention(BusinessDayConvention bdc) {
        swaptionConvention_ = bdc;
        return *this;
    }



}