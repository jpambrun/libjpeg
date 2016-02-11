/*
**
** This is the base class for all predictors, to be used for the
** lossless predictive mode. It performs the prediction, based
** on the mode, in a virtual subclass.
**
** $Id: predictorbase.cpp,v 1.4 2014/11/17 12:15:25 thor Exp $
**
*/

/// Includes
#include "tools/environment.hpp"
#include "codestream/predictorbase.hpp"
#include "codestream/predictor.hpp"
#if ACCUSOFT_CODE
///

/// PredictorBase::CreatePredictor
// Create a predictor of the given preshift and mode.
template<PredictorBase::PredictionMode mode>
PredictorBase *PredictorBase::CreatePredictor(class Environ *env,UBYTE preshift,
						    LONG neutral)
{
  switch(preshift) {
  case 0:
    return new(env) Predictor<mode,0>(neutral);
  case 1:
    return new(env) Predictor<mode,1>(neutral);
  case 2:
    return new(env) Predictor<mode,2>(neutral);
  case 3:
    return new(env) Predictor<mode,3>(neutral);
  case 4:
    return new(env) Predictor<mode,4>(neutral);
  case 5:
    return new(env) Predictor<mode,5>(neutral);
  case 6:
    return new(env) Predictor<mode,6>(neutral);
  case 7:
    return new(env) Predictor<mode,7>(neutral);
  case 8:
    return new(env) Predictor<mode,8>(neutral);
  case 9:
    return new(env) Predictor<mode,9>(neutral);
  case 10:
    return new(env) Predictor<mode,10>(neutral);
  case 11:
    return new(env) Predictor<mode,11>(neutral);
  case 12:
    return new(env) Predictor<mode,12>(neutral);
  case 13:
    return new(env) Predictor<mode,13>(neutral);
  case 14:
    return new(env) Predictor<mode,14>(neutral);
  case 15:
    return new(env) Predictor<mode,15>(neutral);
  case 16:
    return new(env) Predictor<mode,16>(neutral);
  case 17:
    return new(env) Predictor<mode,17>(neutral);
  case 18:
    return new(env) Predictor<mode,18>(neutral);
  case 19:
    return new(env) Predictor<mode,19>(neutral);
  case 20:
    return new(env) Predictor<mode,20>(neutral);
  }
  return NULL;
}
///

/// PredictorBase::CreatePredictor 
// Create a predictor of the given preshift and mode.
PredictorBase *PredictorBase::CreatePredictor(class Environ *env,PredictorBase::PredictionMode mode,UBYTE preshift,LONG neutral)
{
  switch(mode) {
  case None:
    return CreatePredictor<None>(env,preshift,neutral);
  case Left:
    return CreatePredictor<Left>(env,preshift,neutral);
  case Top:
    return CreatePredictor<Top>(env,preshift,neutral);
  case LeftTop:
    return CreatePredictor<LeftTop>(env,preshift,neutral);
  case Linear:
    return CreatePredictor<Linear>(env,preshift,neutral);
  case WeightA:
    return CreatePredictor<WeightA>(env,preshift,neutral);
  case WeightB:
    return CreatePredictor<WeightB>(env,preshift,neutral);
  case Diagonal:
    return CreatePredictor<Diagonal>(env,preshift,neutral);
  case Neutral:
    return CreatePredictor<Neutral>(env,preshift,neutral);
  }
  return NULL;
}
///

/// PredictorBase::CreatePredictorChain
// Create a prediction chain for the given neutral value and
// the given prediction mode. The mode "none" is used to
// indicate the differential mode.
// This requires an array of four predictors to be used for
// life-time management. The first element is the initial predictor.
void PredictorBase::CreatePredictorChain(class Environ *m_pEnviron,class PredictorBase *chain[4],
					 PredictionMode mode,UBYTE preshift,
					 LONG neutral)
{
  if (preshift > 20)
    JPG_THROW(OVERFLOW_PARAMETER,"PredictorBase::CreatePredictorChain",
	      "lossless predictive point transformation value is out of range, no code provisioned for it");
  
  assert(chain[0] == NULL && chain[1] == NULL && chain[2] == NULL && chain[3] == NULL);
  
  switch(mode) {
  case None:
    // This is differential. It only requires a single predictor.
    chain[0] = CreatePredictor(m_pEnviron,mode,preshift,0);
    assert(chain[0]);
    // This predictor stays the same no matter how we move.
    chain[0]->m_pNextRight = chain[0];
    chain[0]->m_pNextDown  = chain[0];
    break;
  case Left:
  case Top:
  case LeftTop:
  case Linear:
  case WeightA:
  case WeightB:
  case Diagonal:
    // It starts with the neutral predictor top left.
    chain[0] = CreatePredictor(m_pEnviron,Neutral,preshift,neutral);
    chain[1] = CreatePredictor(m_pEnviron,Left   ,preshift,0);
    chain[2] = CreatePredictor(m_pEnviron,Top    ,preshift,0);
    chain[3] = CreatePredictor(m_pEnviron,mode   ,preshift,0);
    assert(chain[0] && chain[1] && chain[2] && chain[3]);
    //
    // Now link them in.
    chain[0]->m_pNextRight = chain[1];
    chain[1]->m_pNextRight = chain[1];
    chain[0]->m_pNextDown  = chain[2];
    chain[1]->m_pNextDown  = chain[3];
    chain[2]->m_pNextRight = chain[3];
    chain[2]->m_pNextDown  = chain[2];
    chain[3]->m_pNextRight = chain[3];
    chain[3]->m_pNextDown  = chain[3];
    break;
  default:
    JPG_THROW(INVALID_PARAMETER,"PredictorBase::CreatePredictorChain",
	      "unable to initiate a lossless predictive scan, invalid prediction mode specified");
    break;
  }
}
///

///
#endif