/*
 * AdaptiveMarginals.h --
 *	Adaptive marginals language model (Kneser et al, Eurospeech 97)
 *
 * Copyright (c) 2004 SRI International.  All Rights Reserved.
 *
 * @(#)$Header: /home/srilm/devel/lm/src/RCS/AdaptiveMarginals.h,v 1.1 2004/07/31 22:17:56 stolcke Exp $
 *
 */

#ifndef _AdaptiveMarginals_h
#define _AdaptiveMarginals_h

#include "LM.h"
#include "LHash.h"
#include "Trie.h"

class AdaptiveMarginals: public LM
{
public:
    AdaptiveMarginals(Vocab &vocab, LM &baseLM,
			LM &baseMarginals, LM &adaptMarginals, double beta);

    double beta;		/* adaptation weight parameter */
    Boolean computeRatios;	/* output ratio between adapted and unadapted */

    /*
     * LM interface
     */
    virtual LogP wordProb(VocabIndex word, const VocabIndex *context);
    virtual void *contextID(VocabIndex word, const VocabIndex *context,
							unsigned &length);
    virtual Boolean isNonWord(VocabIndex word);
    virtual void setState(const char *state);

    /*
     * Propagate changes to running state to base model
     */
    virtual Boolean running() { return _running; }
    virtual Boolean running(Boolean newstate)
      { Boolean old = _running; _running = newstate; 
	baseLM.running(newstate); return old; };

    /*
     * Propagate changes to Debug state to component models
     */
    void debugme(unsigned level)
	{ baseLM.debugme(level); baseMarginals.debugme(level);
	  adaptMarginals.debugme(level); Debug::debugme(level); };
    ostream &dout() { return Debug::dout(); };
    ostream &dout(ostream &stream)  /* propagate dout changes to sub-lms */
	{ baseLM.dout(stream); baseMarginals.dout(stream);
	  adaptMarginals.dout(stream); return Debug::dout(stream); };

protected:
    LM &baseLM;			/* unadapted model */
    LM &baseMarginals;		/* unigram marginals of base model */
    LM &adaptMarginals;		/* unigram marginals to adapt to */

    LHash<VocabIndex, LogP> adaptAlphas;
				/* ratios between base and adapted marginals */
    Trie<VocabIndex, LogP> denomProbs;
				/* cached denominators of adapted probs */
    void computeAlphas();	/* precompute all adaptAlphas */
    Boolean haveAlphas;		/* are adaptAlphas computed */
};


#endif /* _AdaptiveMarginals_h */
