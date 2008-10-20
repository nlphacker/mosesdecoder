//#ifndef MAXENTREORDERING_H_
//#define MAXENTREORDERING_H_
#pragma once

#include <string>
#include <vector>
#include "Factor.h"
#include "Phrase.h"
#include "TypeDef.h"
#include "Util.h"
#include "WordsRange.h"
#include "ScoreProducer.h"

#include "LexicalReorderingTable.h"

namespace Moses
{

class Factor;
class Phrase;
class Hypothesis;
class InputType;

using namespace std;

class MaxentReordering : public ScoreProducer {
 public: //types & consts
  typedef int OrientationType; 
  enum Direction {Forward, Backward, Bidirectional, Unidirectional = Backward};
  enum Condition {F,E,C,FE,FEC};
 public: //con- & destructors 
  MaxentReordering(const std::string &filePath, 
		    const std::vector<float>& weights, 
		    Direction direction, 
		    Condition condition, 
		    std::vector< FactorType >& f_factors, 
		    std::vector< FactorType >& e_factors);
  virtual ~MaxentReordering();
 public: //interface
  //inherited
  virtual size_t GetNumScoreComponents() const {
    return m_NumScoreComponents; 
  };
  
  virtual std::string GetScoreProducerDescription() const {
    return "Generic Maxent Reordering Model... overwrite in subclass.";
  };
  //new 
  virtual int             GetNumOrientationTypes() const = 0;
  virtual OrientationType GetOrientationType(Hypothesis*) const = 0;
  
  std::vector<float> CalcScore(Hypothesis* hypothesis) const;
  void InitializeForInput(const InputType& i){
    m_Table->InitializeForInput(i);
  }

	Score GetProb(const Phrase& f, const Phrase& e) const;
  //helpers
  static std::vector<Condition> DecodeCondition(Condition c);
  static std::vector<Direction> DecodeDirection(Direction d);
 private:
  Phrase auxGetContext(const Hypothesis* hypothesis) const;
 private:
  LexicalReorderingTable* m_Table;
  size_t m_NumScoreComponents;
  std::vector< Direction > m_Direction;
  std::vector< Condition > m_Condition;
  bool m_OneScorePerDirection;
  std::vector< FactorType > m_FactorsE, m_FactorsF, m_FactorsC;
  int m_MaxContextLength;
};


class MaxentOrientationReordering : public MaxentReordering {
 private:
  enum {Monotone = 0, Swap = 1, Discontinuous = 2};
 public:
    MaxentOrientationReordering(const std::string &filePath, 
			     const std::vector<float>& w, 
			     Direction direction, 
			     Condition condition, 
			     std::vector< FactorType >& f_factors, 
			     std::vector< FactorType >& e_factors)
      : MaxentReordering(filePath, w, direction, condition, f_factors, e_factors){
	  std::cerr << "Created maxent orientation reordering\n";
  }
 public:
  virtual int GetNumOrientationTypes() const {
    return 3; 
  }
  virtual std::string GetScoreProducerDescription() const {
    return "OrientationMaxentReorderingModel";
  };
  virtual OrientationType GetOrientationType(Hypothesis* currHypothesis) const;
};

}
//#endif /*MAXENTREORDERING_H_*/

