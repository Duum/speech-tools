
 /************************************************************************/
 /*                                                                      */
 /*                Centre for Speech Technology Research                 */
 /*                     University of Edinburgh, UK                      */
 /*                       Copyright (c) 1996,1997                        */
 /*                        All Rights Reserved.                          */
 /*                                                                      */
 /*  Permission is hereby granted, free of charge, to use and distribute */
 /*  this software and its documentation without restriction, including  */
 /*  without limitation the rights to use, copy, modify, merge, publish, */
 /*  distribute, sublicense, and/or sell copies of this work, and to     */
 /*  permit persons to whom this work is furnished to do so, subject to  */
 /*  the following conditions:                                           */
 /*   1. The code must retain the above copyright notice, this list of   */
 /*      conditions and the following disclaimer.                        */
 /*   2. Any modifications must be clearly marked as such.               */
 /*   3. Original authors' names are not deleted.                        */
 /*   4. The authors' names are not used to endorse or promote products  */
 /*      derived from this software without specific prior written       */
 /*      permission.                                                     */
 /*                                                                      */
 /*  THE UNIVERSITY OF EDINBURGH AND THE CONTRIBUTORS TO THIS WORK       */
 /*  DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING     */
 /*  ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT  */
 /*  SHALL THE UNIVERSITY OF EDINBURGH NOR THE CONTRIBUTORS BE LIABLE    */
 /*  FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES   */
 /*  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN  */
 /*  AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,         */
 /*  ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF      */
 /*  THIS SOFTWARE.                                                      */
 /*                                                                      */
 /*************************************************************************/


#ifndef __EST_TDEQ_I_H__
#define __EST_TDEQ_I_H__

/** Instantiate rules for list template.
  * 
  * @author Richard Caley <rjc@cstr.ed.ac.uk>
  * @version $Id: EST_TDequeI.h,v 1.4 1999/06/02 15:43:31 rjc release $
  */

// Instantiation Macros

// the typedef is purely to get the type name through the following macro.

#define Instantiate_TDEQ_T(T, TAG) \
        template class EST_TDeque<T>; \
	ostream& operator << (ostream &st, const EST_TDeque< T > &deq) { return deq.print(st);}


#define Instantiate_TDEQ(T) Instantiate_TDEQ_T(T, T) 

#define Declare_TDEQ_T(T, TAG)  \
	const T *EST_TDeque< T >::Filler=NULL;

#define Declare_TDEQ_Base_T(T, FILLER ,TAG) \
	const T TAG ## _deq_filler=FILLER; \
	const T *EST_TDeque< T >::Filler=& TAG ## _deq_filler;
	

#define Declare_TDEQ_Class_T(T, FILLER,TAG) \
	const T TAG ## _deq_filler(FILLER); \
	const T *EST_TDeque< T >::Filler=& TAG ## _deq_filler;

#define Declare_TDEQ(T) Declare_TDEQ_T(T, T)

#define Declare_TDEQ_Base(T, FILLER)  Declare_TDEQ_Base_T(T, FILLER, T)
#define Declare_TDEQ_Class(T, FILLER) Declare_TDEQ_Class_T(T, FILLER, T)

#endif

