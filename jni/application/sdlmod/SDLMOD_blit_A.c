/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2012 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    Sam Lantinga
    slouken@libsdl.org
*/

#include <memory.h>
#if defined(_DEBUG)
#include <stdio.h>
#endif
#include "SDLMOD_video.h"

/*
  In Visual C, VC6 has mmintrin.h in the "Processor Pack" add-on.
   Checking if _mm_free is #defined in malloc.h is is the only way to
   determine if the Processor Pack is installed, as far as I can tell.
*/

/* Function to check the CPU flags */


/* Functions to perform alpha blended blitting */

/* N->1 blending with per-surface alpha */
static void BlitNto1SurfaceAlpha(SDLMOD_BlitInfo *info)
{
	int width = info->d_width;
	int height = info->d_height;
	uint8_t *src = info->s_pixels;
	int srcskip = info->s_skip;
	uint8_t *dst = info->d_pixels;
	int dstskip = info->d_skip;
	uint8_t *palmap = info->table;
	SDLMOD_PixelFormat *srcfmt = info->src;
	SDLMOD_PixelFormat *dstfmt = info->dst;
	int srcbpp = srcfmt->BytesPerPixel;

	const unsigned A = srcfmt->alpha;

	while ( height-- ) {
	    DUFFS_LOOP4(
	    {
		uint32_t Pixel;
		unsigned sR;
		unsigned sG;
		unsigned sB;
		unsigned dR;
		unsigned dG;
		unsigned dB;
		DISEMBLE_RGB(src, srcbpp, srcfmt, Pixel, sR, sG, sB);
		dR = dstfmt->palette->colors[*dst].r;
		dG = dstfmt->palette->colors[*dst].g;
		dB = dstfmt->palette->colors[*dst].b;
		ALPHA_BLEND(sR, sG, sB, A, dR, dG, dB);
		dR &= 0xff;
		dG &= 0xff;
		dB &= 0xff;
		/* Pack RGB into 8bit pixel */
		if ( palmap == NULL ) {
		    *dst =((dR>>5)<<(3+2))|
			  ((dG>>5)<<(2))|
			  ((dB>>6)<<(0));
		} else {
		    *dst = palmap[((dR>>5)<<(3+2))|
				  ((dG>>5)<<(2))  |
				  ((dB>>6)<<(0))];
		}
		dst++;
		src += srcbpp;
	    },
	    width);
	    src += srcskip;
	    dst += dstskip;
	}
}

/* N->1 blending with pixel alpha */
static void BlitNto1PixelAlpha(SDLMOD_BlitInfo *info)
{
	int width = info->d_width;
	int height = info->d_height;
	uint8_t *src = info->s_pixels;
	int srcskip = info->s_skip;
	uint8_t *dst = info->d_pixels;
	int dstskip = info->d_skip;
	uint8_t *palmap = info->table;
	SDLMOD_PixelFormat *srcfmt = info->src;
	SDLMOD_PixelFormat *dstfmt = info->dst;
	int srcbpp = srcfmt->BytesPerPixel;

	/* FIXME: fix alpha bit field expansion here too? */
	while ( height-- ) {
	    DUFFS_LOOP4(
	    {
		uint32_t Pixel;
		unsigned sR;
		unsigned sG;
		unsigned sB;
		unsigned sA;
		unsigned dR;
		unsigned dG;
		unsigned dB;
		DISEMBLE_RGBA(src,srcbpp,srcfmt,Pixel,sR,sG,sB,sA);
		dR = dstfmt->palette->colors[*dst].r;
		dG = dstfmt->palette->colors[*dst].g;
		dB = dstfmt->palette->colors[*dst].b;
		ALPHA_BLEND(sR, sG, sB, sA, dR, dG, dB);
		dR &= 0xff;
		dG &= 0xff;
		dB &= 0xff;
		/* Pack RGB into 8bit pixel */
		if ( palmap == NULL ) {
		    *dst =((dR>>5)<<(3+2))|
			  ((dG>>5)<<(2))|
			  ((dB>>6)<<(0));
		} else {
		    *dst = palmap[((dR>>5)<<(3+2))|
				  ((dG>>5)<<(2))  |
				  ((dB>>6)<<(0))  ];
		}
		dst++;
		src += srcbpp;
	    },
	    width);
	    src += srcskip;
	    dst += dstskip;
	}
}

/* colorkeyed N->1 blending with per-surface alpha */
static void BlitNto1SurfaceAlphaKey(SDLMOD_BlitInfo *info)
{
	int width = info->d_width;
	int height = info->d_height;
	uint8_t *src = info->s_pixels;
	int srcskip = info->s_skip;
	uint8_t *dst = info->d_pixels;
	int dstskip = info->d_skip;
	uint8_t *palmap = info->table;
	SDLMOD_PixelFormat *srcfmt = info->src;
	SDLMOD_PixelFormat *dstfmt = info->dst;
	int srcbpp = srcfmt->BytesPerPixel;
	uint32_t ckey = srcfmt->colorkey;

	const int A = srcfmt->alpha;

	while ( height-- ) {
	    DUFFS_LOOP(
	    {
		uint32_t Pixel;
		unsigned sR;
		unsigned sG;
		unsigned sB;
		unsigned dR;
		unsigned dG;
		unsigned dB;
		DISEMBLE_RGB(src, srcbpp, srcfmt, Pixel, sR, sG, sB);
		if ( Pixel != ckey ) {
		    dR = dstfmt->palette->colors[*dst].r;
		    dG = dstfmt->palette->colors[*dst].g;
		    dB = dstfmt->palette->colors[*dst].b;
		    ALPHA_BLEND(sR, sG, sB, A, dR, dG, dB);
		    dR &= 0xff;
		    dG &= 0xff;
		    dB &= 0xff;
		    /* Pack RGB into 8bit pixel */
		    if ( palmap == NULL ) {
			*dst =((dR>>5)<<(3+2))|
			      ((dG>>5)<<(2)) |
			      ((dB>>6)<<(0));
		    } else {
			*dst = palmap[((dR>>5)<<(3+2))|
				      ((dG>>5)<<(2))  |
				      ((dB>>6)<<(0))  ];
		    }
		}
		dst++;
		src += srcbpp;
	    },
	    width);
	    src += srcskip;
	    dst += dstskip;
	}
}

















/* fast RGB888->(A)RGB888 blending with surface alpha=128 special case */
static void BlitRGBtoRGBSurfaceAlpha128(SDLMOD_BlitInfo *info)
{
	int width = info->d_width;
	int height = info->d_height;
	uint32_t *srcp = (uint32_t *)info->s_pixels;
	int srcskip = info->s_skip >> 2;
	uint32_t *dstp = (uint32_t *)info->d_pixels;
	int dstskip = info->d_skip >> 2;

	while(height--) {
	    DUFFS_LOOP4({
		    uint32_t s = *srcp++;
		    uint32_t d = *dstp;
		    *dstp++ = ((((s & 0x00fefefe) + (d & 0x00fefefe)) >> 1)
			       + (s & d & 0x00010101)) | 0xff000000;
	    }, width);
	    srcp += srcskip;
	    dstp += dstskip;
	}
}

/* fast RGB888->(A)RGB888 blending with surface alpha */
static void BlitRGBtoRGBSurfaceAlpha(SDLMOD_BlitInfo *info)
{
	unsigned alpha = info->src->alpha;
	if(alpha == 128) {
		BlitRGBtoRGBSurfaceAlpha128(info);
	} else {
		int width = info->d_width;
		int height = info->d_height;
		uint32_t *srcp = (uint32_t *)info->s_pixels;
		int srcskip = info->s_skip >> 2;
		uint32_t *dstp = (uint32_t *)info->d_pixels;
		int dstskip = info->d_skip >> 2;
		uint32_t s;
		uint32_t d;
		uint32_t s1;
		uint32_t d1;

		while(height--) {
			DUFFS_LOOP_DOUBLE2({
				/* One Pixel Blend */
				s = *srcp;
				d = *dstp;
				s1 = s & 0xff00ff;
				d1 = d & 0xff00ff;
				d1 = (d1 + ((s1 - d1) * alpha >> 8))
				     & 0xff00ff;
				s &= 0xff00;
				d &= 0xff00;
				d = (d + ((s - d) * alpha >> 8)) & 0xff00;
				*dstp = d1 | d | 0xff000000;
				++srcp;
				++dstp;
			},{
			        /* Two Pixels Blend */
				s = *srcp;
				d = *dstp;
				s1 = s & 0xff00ff;
				d1 = d & 0xff00ff;
				d1 += (s1 - d1) * alpha >> 8;
				d1 &= 0xff00ff;
				     
				s = ((s & 0xff00) >> 8) | 
					((srcp[1] & 0xff00) << 8);
				d = ((d & 0xff00) >> 8) |
					((dstp[1] & 0xff00) << 8);
				d += (s - d) * alpha >> 8;
				d &= 0x00ff00ff;
				
				*dstp++ = d1 | ((d << 8) & 0xff00) | 0xff000000;
				++srcp;
				
			        s1 = *srcp;
				d1 = *dstp;
				s1 &= 0xff00ff;
				d1 &= 0xff00ff;
				d1 += (s1 - d1) * alpha >> 8;
				d1 &= 0xff00ff;
				
				*dstp = d1 | ((d >> 8) & 0xff00) | 0xff000000;
				++srcp;
				++dstp;
			}, width);
			srcp += srcskip;
			dstp += dstskip;
		}
	}
}

/* fast ARGB888->(A)RGB888 blending with pixel alpha */
static void BlitRGBtoRGBPixelAlpha(SDLMOD_BlitInfo *info)
{
	int width = info->d_width;
	int height = info->d_height;
	uint32_t *srcp = (uint32_t *)info->s_pixels;
	int srcskip = info->s_skip >> 2;
	uint32_t *dstp = (uint32_t *)info->d_pixels;
	int dstskip = info->d_skip >> 2;

	while(height--) {
	    DUFFS_LOOP4({
		uint32_t dalpha;
		uint32_t d;
		uint32_t s1;
		uint32_t d1;
		uint32_t s = *srcp;
		uint32_t alpha = s >> 24;
		/* FIXME: Here we special-case opaque alpha since the
		   compositioning used (>>8 instead of /255) doesn't handle
		   it correctly. Also special-case alpha=0 for speed?
		   Benchmark this! */
		if(alpha) {   
		  if(alpha == SDLMOD_ALPHA_OPAQUE) {
		    *dstp = (s & 0x00ffffff) | (*dstp & 0xff000000);
		  } else {
		    /*
		     * take out the middle component (green), and process
		     * the other two in parallel. One multiply less.
		     */
		    d = *dstp;
		    dalpha = d & 0xff000000;
		    s1 = s & 0xff00ff;
		    d1 = d & 0xff00ff;
		    d1 = (d1 + ((s1 - d1) * alpha >> 8)) & 0xff00ff;
		    s &= 0xff00;
		    d &= 0xff00;
		    d = (d + ((s - d) * alpha >> 8)) & 0xff00;
		    *dstp = d1 | d | dalpha;
		  }
		}
		++srcp;
		++dstp;
	    }, width);
	    srcp += srcskip;
	    dstp += dstskip;
	}
}












/* 16bpp special case for per-surface alpha=50%: blend 2 pixels in parallel */

/* blend a single 16 bit pixel at 50% */
#define BLEND16_50(d, s, mask)						\
	((((s & mask) + (d & mask)) >> 1) + (s & d & (~mask & 0xffff)))

/* blend two 16 bit pixels at 50% */
#define BLEND2x16_50(d, s, mask)					     \
	(((s & (mask | mask << 16)) >> 1) + ((d & (mask | mask << 16)) >> 1) \
	 + (s & d & (~(mask | mask << 16))))

static void Blit16to16SurfaceAlpha128(SDLMOD_BlitInfo *info, uint16_t mask)
{
	int width = info->d_width;
	int height = info->d_height;
	uint16_t *srcp = (uint16_t *)info->s_pixels;
	int srcskip = info->s_skip >> 1;
	uint16_t *dstp = (uint16_t *)info->d_pixels;
	int dstskip = info->d_skip >> 1;

	while(height--) {
		if(((uintptr_t)srcp ^ (uintptr_t)dstp) & 2) {
			/*
			 * Source and destination not aligned, pipeline it.
			 * This is mostly a win for big blits but no loss for
			 * small ones
			 */
			uint32_t prev_sw;
			int w = width;

			/* handle odd destination */
			if((uintptr_t)dstp & 2) {
				uint16_t d = *dstp, s = *srcp;
				*dstp = BLEND16_50(d, s, mask);
				dstp++;
				srcp++;
				w--;
			}
			srcp++;	/* srcp is now 32-bit aligned */

			/* bootstrap pipeline with first halfword */
			prev_sw = ((uint32_t *)srcp)[-1];

			while(w > 1) {
				uint32_t sw, dw, s;
				sw = *(uint32_t *)srcp;
				dw = *(uint32_t *)dstp;
#if SDLMOD_BYTEORDER == SDLMOD_BIG_ENDIAN
				s = (prev_sw << 16) + (sw >> 16);
#else
				s = (prev_sw >> 16) + (sw << 16);
#endif
				prev_sw = sw;
				*(uint32_t *)dstp = BLEND2x16_50(dw, s, mask);
				dstp += 2;
				srcp += 2;
				w -= 2;
			}

			/* final pixel if any */
			if(w) {
				uint16_t d = *dstp, s;
#if SDLMOD_BYTEORDER == SDLMOD_BIG_ENDIAN
				s = (uint16_t)prev_sw;
#else
				s = (uint16_t)(prev_sw >> 16);
#endif
				*dstp = BLEND16_50(d, s, mask);
				srcp++;
				dstp++;
			}
			srcp += srcskip - 1;
			dstp += dstskip;
		} else {
			/* source and destination are aligned */
			int w = width;

			/* first odd pixel? */
			if((uintptr_t)srcp & 2) {
				uint16_t d = *dstp, s = *srcp;
				*dstp = BLEND16_50(d, s, mask);
				srcp++;
				dstp++;
				w--;
			}
			/* srcp and dstp are now 32-bit aligned */

			while(w > 1) {
				uint32_t sw = *(uint32_t *)srcp;
				uint32_t dw = *(uint32_t *)dstp;
				*(uint32_t *)dstp = BLEND2x16_50(dw, sw, mask);
				srcp += 2;
				dstp += 2;
				w -= 2;
			}

			/* last odd pixel? */
			if(w) {
				uint16_t d = *dstp, s = *srcp;
				*dstp = BLEND16_50(d, s, mask);
				srcp++;
				dstp++;
			}
			srcp += srcskip;
			dstp += dstskip;
		}
	}
}


/* fast RGB565->RGB565 blending with surface alpha */
static void Blit565to565SurfaceAlpha(SDLMOD_BlitInfo *info)
{
	unsigned alpha = info->src->alpha;
	if(alpha == 128) {
		Blit16to16SurfaceAlpha128(info, 0xf7de);
	} else {
		int width = info->d_width;
		int height = info->d_height;
		uint16_t *srcp = (uint16_t *)info->s_pixels;
		int srcskip = info->s_skip >> 1;
		uint16_t *dstp = (uint16_t *)info->d_pixels;
		int dstskip = info->d_skip >> 1;
		alpha >>= 3;	/* downscale alpha to 5 bits */

		while(height--) {
			DUFFS_LOOP4({
				uint32_t s = *srcp++;
				uint32_t d = *dstp;
				/*
				 * shift out the middle component (green) to
				 * the high 16 bits, and process all three RGB
				 * components at the same time.
				 */
				s = (s | s << 16) & 0x07e0f81f;
				d = (d | d << 16) & 0x07e0f81f;
				d += (s - d) * alpha >> 5;
				d &= 0x07e0f81f;
				*dstp++ = (uint16_t)(d | d >> 16);
			}, width);
			srcp += srcskip;
			dstp += dstskip;
		}
	}
}

/* fast RGB555->RGB555 blending with surface alpha */
static void Blit555to555SurfaceAlpha(SDLMOD_BlitInfo *info)
{
	unsigned alpha = info->src->alpha; /* downscale alpha to 5 bits */
	if(alpha == 128) {
		Blit16to16SurfaceAlpha128(info, 0xfbde);
	} else {
		int width = info->d_width;
		int height = info->d_height;
		uint16_t *srcp = (uint16_t *)info->s_pixels;
		int srcskip = info->s_skip >> 1;
		uint16_t *dstp = (uint16_t *)info->d_pixels;
		int dstskip = info->d_skip >> 1;
		alpha >>= 3;		/* downscale alpha to 5 bits */

		while(height--) {
			DUFFS_LOOP4({
				uint32_t s = *srcp++;
				uint32_t d = *dstp;
				/*
				 * shift out the middle component (green) to
				 * the high 16 bits, and process all three RGB
				 * components at the same time.
				 */
				s = (s | s << 16) & 0x03e07c1f;
				d = (d | d << 16) & 0x03e07c1f;
				d += (s - d) * alpha >> 5;
				d &= 0x03e07c1f;
				*dstp++ = (uint16_t)(d | d >> 16);
			}, width);
			srcp += srcskip;
			dstp += dstskip;
		}
	}
}

/* fast ARGB8888->RGB565 blending with pixel alpha */
static void BlitARGBto565PixelAlpha(SDLMOD_BlitInfo *info)
{
	int width = info->d_width;
	int height = info->d_height;
	uint32_t *srcp = (uint32_t *)info->s_pixels;
	int srcskip = info->s_skip >> 2;
	uint16_t *dstp = (uint16_t *)info->d_pixels;
	int dstskip = info->d_skip >> 1;

	while(height--) {
	    DUFFS_LOOP4({
		uint32_t s = *srcp;
		unsigned alpha = s >> 27; /* downscale alpha to 5 bits */
		/* FIXME: Here we special-case opaque alpha since the
		   compositioning used (>>8 instead of /255) doesn't handle
		   it correctly. Also special-case alpha=0 for speed?
		   Benchmark this! */
		if(alpha) {   
		  if(alpha == (SDLMOD_ALPHA_OPAQUE >> 3)) {
		    *dstp = (uint16_t)((s >> 8 & 0xf800) + (s >> 5 & 0x7e0) + (s >> 3  & 0x1f));
		  } else {
		    uint32_t d = *dstp;
		    /*
		     * convert source and destination to G0RAB65565
		     * and blend all components at the same time
		     */
		    s = ((s & 0xfc00) << 11) + (s >> 8 & 0xf800)
		      + (s >> 3 & 0x1f);
		    d = (d | d << 16) & 0x07e0f81f;
		    d += (s - d) * alpha >> 5;
		    d &= 0x07e0f81f;
		    *dstp = (uint16_t)(d | d >> 16);
		  }
		}
		srcp++;
		dstp++;
	    }, width);
	    srcp += srcskip;
	    dstp += dstskip;
	}
}

/* fast ARGB8888->RGB555 blending with pixel alpha */
static void BlitARGBto555PixelAlpha(SDLMOD_BlitInfo *info)
{
	int width = info->d_width;
	int height = info->d_height;
	uint32_t *srcp = (uint32_t *)info->s_pixels;
	int srcskip = info->s_skip >> 2;
	uint16_t *dstp = (uint16_t *)info->d_pixels;
	int dstskip = info->d_skip >> 1;

	while(height--) {
	    DUFFS_LOOP4({
		unsigned alpha;
		uint32_t s = *srcp;
		alpha = s >> 27; /* downscale alpha to 5 bits */
		/* FIXME: Here we special-case opaque alpha since the
		   compositioning used (>>8 instead of /255) doesn't handle
		   it correctly. Also special-case alpha=0 for speed?
		   Benchmark this! */
		if(alpha) {   
		  if(alpha == (SDLMOD_ALPHA_OPAQUE >> 3)) {
		    *dstp = (uint16_t)((s >> 9 & 0x7c00) + (s >> 6 & 0x3e0) + (s >> 3  & 0x1f));
		  } else {
		    uint32_t d = *dstp;
		    /*
		     * convert source and destination to G0RAB65565
		     * and blend all components at the same time
		     */
		    s = ((s & 0xf800) << 10) + (s >> 9 & 0x7c00)
		      + (s >> 3 & 0x1f);
		    d = (d | d << 16) & 0x03e07c1f;
		    d += (s - d) * alpha >> 5;
		    d &= 0x03e07c1f;
		    *dstp = (uint16_t)(d | d >> 16);
		  }
		}
		srcp++;
		dstp++;
	    }, width);
	    srcp += srcskip;
	    dstp += dstskip;
	}
}

/* General (slow) N->N blending with per-surface alpha */
static void BlitNtoNSurfaceAlpha(SDLMOD_BlitInfo *info)
{
	int width = info->d_width;
	int height = info->d_height;
	uint8_t *src = info->s_pixels;
	int srcskip = info->s_skip;
	uint8_t *dst = info->d_pixels;
	int dstskip = info->d_skip;
	SDLMOD_PixelFormat *srcfmt = info->src;
	SDLMOD_PixelFormat *dstfmt = info->dst;
	int srcbpp = srcfmt->BytesPerPixel;
	int dstbpp = dstfmt->BytesPerPixel;
	unsigned sA = srcfmt->alpha;
	unsigned dA = dstfmt->Amask ? SDLMOD_ALPHA_OPAQUE : 0;

	if(sA) {
	  while ( height-- ) {
	    DUFFS_LOOP4(
	    {
		uint32_t Pixel;
		unsigned sR;
		unsigned sG;
		unsigned sB;
		unsigned dR;
		unsigned dG;
		unsigned dB;
		DISEMBLE_RGB(src, srcbpp, srcfmt, Pixel, sR, sG, sB);
		DISEMBLE_RGB(dst, dstbpp, dstfmt, Pixel, dR, dG, dB);
		ALPHA_BLEND(sR, sG, sB, sA, dR, dG, dB);
		ASSEMBLE_RGBA(dst, dstbpp, dstfmt, dR, dG, dB, dA);
		src += srcbpp;
		dst += dstbpp;
	    },
	    width);
	    src += srcskip;
	    dst += dstskip;
	  }
	}
}

/* General (slow) colorkeyed N->N blending with per-surface alpha */
static void BlitNtoNSurfaceAlphaKey(SDLMOD_BlitInfo *info)
{
	int width = info->d_width;
	int height = info->d_height;
	uint8_t *src = info->s_pixels;
	int srcskip = info->s_skip;
	uint8_t *dst = info->d_pixels;
	int dstskip = info->d_skip;
	SDLMOD_PixelFormat *srcfmt = info->src;
	SDLMOD_PixelFormat *dstfmt = info->dst;
	uint32_t ckey = srcfmt->colorkey;
	int srcbpp = srcfmt->BytesPerPixel;
	int dstbpp = dstfmt->BytesPerPixel;
	unsigned sA = srcfmt->alpha;
	unsigned dA = dstfmt->Amask ? SDLMOD_ALPHA_OPAQUE : 0;

	while ( height-- ) {
	    DUFFS_LOOP4(
	    {
		uint32_t Pixel;
		unsigned sR;
		unsigned sG;
		unsigned sB;
		unsigned dR;
		unsigned dG;
		unsigned dB;
		RETRIEVE_RGB_PIXEL(src, srcbpp, Pixel);
		if(sA && Pixel != ckey) {
		    RGB_FROM_PIXEL(Pixel, srcfmt, sR, sG, sB);
		    DISEMBLE_RGB(dst, dstbpp, dstfmt, Pixel, dR, dG, dB);
		    ALPHA_BLEND(sR, sG, sB, sA, dR, dG, dB);
		    ASSEMBLE_RGBA(dst, dstbpp, dstfmt, dR, dG, dB, dA);
		}
		src += srcbpp;
		dst += dstbpp;
	    },
	    width);
	    src += srcskip;
	    dst += dstskip;
	}
}

/* General (slow) N->N blending with pixel alpha */
static void BlitNtoNPixelAlpha(SDLMOD_BlitInfo *info)
{
	int width = info->d_width;
	int height = info->d_height;
	uint8_t *src = info->s_pixels;
	int srcskip = info->s_skip;
	uint8_t *dst = info->d_pixels;
	int dstskip = info->d_skip;
	SDLMOD_PixelFormat *srcfmt = info->src;
	SDLMOD_PixelFormat *dstfmt = info->dst;

	int  srcbpp;
	int  dstbpp;

	/* Set up some basic variables */
	srcbpp = srcfmt->BytesPerPixel;
	dstbpp = dstfmt->BytesPerPixel;

	/* FIXME: for 8bpp source alpha, this doesn't get opaque values
	   quite right. for <8bpp source alpha, it gets them very wrong
	   (check all macros!)
	   It is unclear whether there is a good general solution that doesn't
	   need a branch (or a divide). */
#if defined(_DEBUG)
	fprintf(stderr, "srcbpp == %d\n", srcbpp);
	fprintf(stderr, "dstbpp == %d\n", dstbpp);
	fprintf(stderr, "src == %x\n", src);
	fprintf(stderr, "dst == %x\n", dst);
#endif
	while ( height-- ) {
	    DUFFS_LOOP4(
	    {
		uint32_t Pixel;
		unsigned sR;
		unsigned sG;
		unsigned sB;
		unsigned dR;
		unsigned dG;
		unsigned dB;
		unsigned sA;
		unsigned dA;
		DISEMBLE_RGBA(src, srcbpp, srcfmt, Pixel, sR, sG, sB, sA);
		if(sA) {
		  DISEMBLE_RGBA(dst, dstbpp, dstfmt, Pixel, dR, dG, dB, dA);
		  ALPHA_BLEND(sR, sG, sB, sA, dR, dG, dB);
		  ASSEMBLE_RGBA(dst, dstbpp, dstfmt, dR, dG, dB, dA);
		}
		src += srcbpp;
		dst += dstbpp;
	    },
	    width);
	    src += srcskip;
	    dst += dstskip;
	}
}


SDLMOD_loblit SDLMOD_CalculateAlphaBlit(SDLMOD_Surface *surface, int blit_index)
{
    SDLMOD_PixelFormat *sf = surface->format;
    SDLMOD_PixelFormat *df = surface->map->dst->format;

    if(sf->Amask == 0) {
	if((surface->flags & SDLMOD_SRCCOLORKEY) == SDLMOD_SRCCOLORKEY) {
	    if(df->BytesPerPixel == 1)
			return &BlitNto1SurfaceAlphaKey;
	    else
            return &BlitNtoNSurfaceAlphaKey;
	} else {
	    /* Per-surface alpha blits */
	    switch(df->BytesPerPixel) {
	    case 1:
		return &BlitNto1SurfaceAlpha;

	    case 2:
		if(surface->map->identity) {
		    if(df->Gmask == 0x7e0)
		    {
			return &Blit565to565SurfaceAlpha;
		    }
		    else if(df->Gmask == 0x3e0)
		    {
			return &Blit555to555SurfaceAlpha;
		    }
		}
		return &BlitNtoNSurfaceAlpha;

	    case 4:
		if(sf->Rmask == df->Rmask
		   && sf->Gmask == df->Gmask
		   && sf->Bmask == df->Bmask
		   && sf->BytesPerPixel == 4)
		{
			if((sf->Rmask | sf->Gmask | sf->Bmask) == 0xffffff)
			{
				return &BlitRGBtoRGBSurfaceAlpha;
			}
		}
			return &BlitNtoNSurfaceAlpha;

	    case 3:
	    default:
		return &BlitNtoNSurfaceAlpha;
	    }
	}
    } else {
	/* Per-pixel alpha blits */
	switch(df->BytesPerPixel) {
	case 1:
	    return &BlitNto1PixelAlpha;

	case 2:
	    if(sf->BytesPerPixel == 4 && sf->Amask == 0xff000000
	       && sf->Gmask == 0xff00
	       && ((sf->Rmask == 0xff && df->Rmask == 0x1f)
		   || (sf->Bmask == 0xff && df->Bmask == 0x1f))) {
		if(df->Gmask == 0x7e0)
		    return &BlitARGBto565PixelAlpha;
		else if(df->Gmask == 0x3e0)
		    return &BlitARGBto555PixelAlpha;
	    }
	    return &BlitNtoNPixelAlpha;

	case 4:
	    if(sf->Rmask == df->Rmask
	       && sf->Gmask == df->Gmask
	       && sf->Bmask == df->Bmask
	       && sf->BytesPerPixel == 4)
	    {
		if(sf->Amask == 0xff000000)
		{
			return &BlitRGBtoRGBPixelAlpha;
		}
	    }
		return &BlitNtoNPixelAlpha;

	case 3:
	default:
	    return &BlitNtoNPixelAlpha;
	}
    }
}

