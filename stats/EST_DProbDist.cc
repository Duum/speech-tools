/*************************************************************************/
/*                                                                       */
/*                Centre for Speech Technology Research                  */
/*                     University of Edinburgh, UK                       */
/*                         Copyright (c) 1996                            */
/*                        All Rights Reserved.                           */
/*                                                                       */
/*  Permission is hereby granted, free of charge, to use and distribute  */
/*  this software and its documentation without restriction, including   */
/*  without limitation the rights to use, copy, modify, merge, publish,  */
/*  distribute, sublicense, and/or sell copies of this work, and to      */
/*  permit persons to whom this work is furnished to do so, subject to   */
/*  the following conditions:                                            */
/*   1. The code must retain the above copyright notice, this list of    */
/*      conditions and the following disclaimer.                         */
/*   2. Any modifications must be clearly marked as such.                */
/*   3. Original authors' names are not deleted.                         */
/*   4. The authors' names are not used to endorse or promote products   */
/*      derived from this software without specific prior written        */
/*      permission.                                                      */
/*                                                                       */
/*  THE UNIVERSITY OF EDINBURGH AND THE CONTRIBUTORS TO THIS WORK        */
/*  DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING      */
/*  ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT   */
/*  SHALL THE UNIVERSITY OF EDINBURGH NOR THE CONTRIBUTORS BE LIABLE     */
/*  FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES    */
/*  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN   */
/*  AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,          */
/*  ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF       */
/*  THIS SOFTWARE.                                                       */
/*                                                                       */
/*************************************************************************/
/*                         Author :  Alan W Black                        */
/*                         Date   :  July 1996                           */
/*-----------------------------------------------------------------------*/
/*                                                                       */
/* Simple statistics (for discrete probability distributions             */
/*                                                                       */
/*=======================================================================*/
#include <iostream.h>
#include <fstream.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "EST_String.h"
#include "EST_TKVL.h"
#include "EST_simplestats.h"

EST_DiscreteProbDistribution::EST_DiscreteProbDistribution(const EST_Discrete *d,
		 const double n_samples, const EST_DVector &counts)
{
    type = tprob_discrete;
    discrete = d;
    num_samples = n_samples;

    icounts = counts;
}

EST_DiscreteProbDistribution::EST_DiscreteProbDistribution(const EST_DiscreteProbDistribution &b)
{
    copy(b);
}

void EST_DiscreteProbDistribution::copy(const EST_DiscreteProbDistribution &b)
{
    type = b.type;
    num_samples = b.num_samples;
    discrete = b.discrete;
    icounts = b.icounts;
    scounts = b.scounts;
}

void EST_DiscreteProbDistribution::clear(void)
{
    icounts.resize(0);
}

void EST_DiscreteProbDistribution::init(void)
{ 
    type = tprob_string; 
    num_samples = 0;
    discrete = 0;
}

bool EST_DiscreteProbDistribution::init(const EST_StrList &vocab)
{
    int i;
    clear();
    type = tprob_discrete;
    num_samples = 0;
    discrete = new EST_Discrete(vocab);

    icounts.resize(vocab.length());
    for (i=0; i<icounts.length(); i++)
	icounts.a_no_check(i) = 0;

    return true;
}

void EST_DiscreteProbDistribution::init(const EST_Discrete *d)
{
    int i;
    clear();
    type = tprob_discrete;
    num_samples = 0;
    discrete = d;
    icounts.resize(d->length());
    for (i=0; i<icounts.length(); i++)
	icounts.a_no_check(i) = 0;
}

void EST_DiscreteProbDistribution::cumulate(const int i,double count)
{
    icounts[i] += count;
    num_samples += count;
}

void EST_DiscreteProbDistribution::cumulate(const EST_String &s,double count)
{
    EST_Litem *p;

    if (type == tprob_discrete)
    {
	int idx = discrete->index(s);
	icounts[idx] += count;
    }
    else // its a (slow) string type 
    {
	for (p=scounts.list.head(); p != 0; p=next(p))
	{
	    if (scounts.list(p).k == s)
	    {
		scounts.list(p).v += count; 
		break;
	    }
	}
	if (p == 0) // first occurence
	    scounts.add_item(s,count,1);  // add without search
    }
    num_samples += count;
}

const EST_String &EST_DiscreteProbDistribution::most_probable(double *prob) const
{
    EST_Litem *p,*t;
    double max = 0;

    if (type == tprob_discrete)
    {
	int i,pt=-1;
	for (i=0; i < icounts.length(); i++)
	    if (icounts.a_no_check(i) > max)
	    {
		pt = i;
		max = icounts.a_no_check(i);
	    }
	if (max == 0)
	{
	    if(prob != NULL)
		*prob = 0.0;
	    return EST_String::Empty;
	}
	else
	{
	    if(prob != NULL)
		*prob = probability(pt);
	    return discrete->name(pt);
	}
    }
    else
    {
	t = 0;
	for (p=scounts.list.head(); p != 0; p=next(p))
	    if (scounts.list(p).v > max)
	    {
		t = p;
		max = scounts.list(p).v;
	    }
	if (max == 0)
	{
	    if(prob != NULL)
		*prob = 0.0;
	    return EST_String::Empty;
	}
	else
	{
	    if(prob != NULL)
		*prob = (double)max/num_samples;
	    return scounts.list(t).k;
	}
    }
}

double EST_DiscreteProbDistribution::probability(const EST_String &s) const
{
    if (frequency(s) == 0.0)
	return 0.0;
    else
	return (double)frequency(s)/num_samples;
}

double EST_DiscreteProbDistribution::probability(const int i) const
{
    if (frequency(i) == 0.0)
	return 0.0;
    else
	return (double)frequency(i)/num_samples;
}

double EST_DiscreteProbDistribution::frequency(const EST_String &s) const
{
    if (type == tprob_discrete)
	return icounts.a_no_check(discrete->index(s));
    else
	return  scounts.val_def(s,0);
}

double EST_DiscreteProbDistribution::frequency(const int i) const
{
    if (type == tprob_discrete)
	return icounts(i);
    else
    {
	cerr << "ProbDistribution: can't access string type pd with int\n";
	return 0;
    }
}

void EST_DiscreteProbDistribution::set_frequency(const EST_String &s,double c)
{
    if (type == tprob_discrete)
    {
	num_samples -= icounts.a_no_check(discrete->index(s));
	num_samples += c;
	icounts.a_no_check(discrete->index(s)) = c;
    }
    else
    {
	num_samples -= scounts.val_def(s,0);
	num_samples += c;
	scounts.add_item(s,c);
    }
}

void EST_DiscreteProbDistribution::set_frequency(int i,double c)
{
    if (type == tprob_discrete)
    {
	num_samples -= icounts[i];
	num_samples += c;
	icounts[i] = c;
    }
    else
    {
	cerr << "ProbDistribution: can't access string type pd with int\n";
    }

}


void EST_DiscreteProbDistribution::override_frequency(const EST_String &s,double c)
{
    if (type == tprob_discrete)
	icounts.a_no_check(discrete->index(s)) = c;
    else
	scounts.add_item(s,c);
}

void EST_DiscreteProbDistribution::override_frequency(int i,double c)
{
    if (type == tprob_discrete)
	icounts[i] = c;
    else
	cerr << "ProbDistribution: can't access string type pd with int\n";
}

double EST_DiscreteProbDistribution::entropy() const
{
    // Returns the entropy of the current distribution
    double e=0.0;
    EST_Litem *p;
    int i;

    if (type == tprob_discrete)
    {
	for (i=0; i < icounts.length(); i++)
	{
	    double prob = icounts.a_no_check(i)/num_samples;
	    if (prob != 0.0)
		e += prob * log(prob);  /* log10(prob)/log10(2) */
	}
    }
    else
    {
	for (p=scounts.list.head(); p != 0; p=next(p))
	{
	    double prob = scounts.list(p).v/num_samples;
	    if (prob != 0.0)
		e += prob * log(prob);  /* log10(prob)/log10(2) */
	}
    }

    return -e;

}

//  For iterating through members of a probability distribution
int EST_DiscreteProbDistribution::item_start(void) const
{
    if (type == tprob_discrete)
	return 0;
    else
	return (int)scounts.list.head();
}

int EST_DiscreteProbDistribution::item_end(int idx) const
{
    if (type == tprob_discrete)
	return (idx >= icounts.length());
    else
	return ((EST_Litem *)idx == 0);
}

int EST_DiscreteProbDistribution::item_next(int idx) const
{
    if (type == tprob_discrete)
	return ++idx;
    else
	return (int)next((EST_Litem *)idx);
}

const EST_String &EST_DiscreteProbDistribution::item_name(int idx) const
{
    if (type == tprob_discrete)
	return discrete->name(idx);
    else
	return scounts.list((EST_Litem *)idx).k;
}

void EST_DiscreteProbDistribution::item_freq(int idx,EST_String &s,double &freq) const
{
    if (type == tprob_discrete)
    {
	s = discrete->name(idx);
	freq = icounts(idx);
    }
    else
    {
	s = scounts.list((EST_Litem *)idx).k;
	freq = scounts.list((EST_Litem *)idx).v;
    }
}

void EST_DiscreteProbDistribution::item_prob(int idx,EST_String &s,double &prob) const
{
    if (type == tprob_discrete)
    {
	prob = probability(idx);
	s = discrete->name(idx);
    }
    else
    {
	s = scounts.list((EST_Litem *)idx).k;
	prob = (double)scounts.list((EST_Litem *)idx).v/num_samples;
    }
}

ostream & operator<<(ostream &s, const EST_DiscreteProbDistribution &pd)
{
    // Output best with probabilities
    int i;
    double prob;
    double sum=0;
    EST_String name;
 
    s << "(";
    for (i=pd.item_start(); !pd.item_end(i); i=pd.item_next(i))
    {
	pd.item_prob(i,name,prob);
	s << "(" << name << "=" << prob << ") ";
	sum+=prob;
    }
    s << "best=" << pd.most_probable(&prob) << " samples=" 
      << pd.samples() << " sum=" << sum << ")";
    return s;
}

EST_DiscreteProbDistribution &EST_DiscreteProbDistribution::operator=(const EST_DiscreteProbDistribution &a)
{
    // I'd much rather this was never called
    copy(a);
    return *this;
}

