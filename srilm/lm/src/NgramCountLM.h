/*
 * NgramCountLM.h --
 *	LM based on interpolated N-gram counts
 *
 * Copyright (c) 2006-2007 SRI International.  All Rights Reserved.
 *
 * @(#)$Header: /home/srilm/devel/lm/src/RCS/NgramCountLM.h,v 1.6 2007/01/24 19:47:10 stolcke Exp $
 *
 */

#ifndef _NgramCountLM_h_
#define _NgramCountLM_h_

#include <stdio.h>

#include "LM.h"
#include "NgramStats.h"
#include "Array.h"

const unsigned defaultNgramCountOrder = 3;

class NgramCountLM: public LM
{
public:
    NgramCountLM(Vocab &vocab, unsigned order = defaultNgramCountOrder);
    virtual ~NgramCountLM();

    /*
     * LM interface
     */
    virtual LogP wordProb(VocabIndex word, const VocabIndex *context);
    virtual void *contextID(VocabIndex word, const VocabIndex *context,
							unsigned &length);

    virtual Boolean read(File &file, Boolean limitVocab = false);
    virtual Boolean write(File &file);

    /*
     * Statistics
     */
    virtual void memStats(MemStats &stats);

    /*
     * Low-level access
     */
    void clear();			/* remove all parameters */

    /*
     * Propagate changes to Debug state to ngram counts
     */
    void debugme(unsigned level)
	{ ngramCounts.debugme(level); Debug::debugme(level); };
    ostream &dout() { return Debug::dout(); };
    ostream &dout(ostream &stream)
	{ ngramCounts.dout(stream); return Debug::dout(stream); };

    /*
     * Estimation
     */
    Boolean estimate(NgramStats &stats);

    unsigned maxEMiters;		/* max number of EM iterations */
    double minEMdelta;			/* min log likelihood delta */

    Boolean writeCounts;		/* whether to write() counts */

protected:
    unsigned order;			/* max ngram length */
    unsigned numWeights;		/* max context count with weights */
    char *countsName;			/* file or dir name for counts */
    Boolean useGoogle;			/* use google format counts */

    Array<Array<Prob> > mixWeights;	/* mixture weight matrix */
    Array<Array<Prob> > mixCounts;	/* posterior sufficient stats  */
    Array<Array<Prob> > mixCountTotals;	/* posterior sufficient stats  */
    NgramStats ngramCounts;		/* ngram count trie */

    VocabIndex *ngramBuffer;		
    VocabIndex *setupNgram(VocabIndex word, const VocabIndex *context);

    Count totalCount;
    Count vocabSize;
    Count countModulus;
    void computeTotals();		/* compute totalCount and vocabSize */

    Boolean training;			/* estimation mode */
    LogP wordProbTrain(VocabIndex word, const VocabIndex *context);
    					/* wordProb version used in training */
};

#endif /* _NgramCountLM_h_ */
