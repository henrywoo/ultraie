// UltraIE - Yet Another IE Add-on
// Copyright (C) 2006-2010
// Simon Wu Fuheng (simonwoo2000 AT gmail.com), Singapore
// Homepage: http://www.linkedin.com/in/simonwoo
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

��# i n c l u d e   " 6 b e e s _ s t r . h "  
 # i n c l u d e   < a t l b a s e . h >  
 # i n c l u d e   < a t l s t r . h >  
  
 # i n c l u d e   < s t r s a f e . h > / /   S t r i n g C c h P r i n t f W  
 # i n c l u d e   < t i m e . h >  
  
 # i n c l u d e   < a l g o r i t h m > / / r e m o v e _ i f  
 # i n c l u d e   < f u n c t i o n a l >  
  
  
  
 # i f n d e f   _ _ 6 B E E U P L O A D E R _ _  
  
 # i n c l u d e   < p c r e + + . h >  
 # i f d e f   N D E B U G  
 # p r a g m a   c o m m e n t ( l i b , " p c r e + + . l i b " )  
 # e l s e  
 # p r a g m a   c o m m e n t ( l i b , " p c r e + + d . l i b " )  
 # e n d i f  
 u s i n g   n a m e s p a c e   p c r e p p ;  
  
 # e n d i f  
  
 n a m e s p a c e   _ 6 b e e s _ s t r {  
      
  
     b o o l   s t r r e p l a c e ( w s t r i n g &   s o u r c e ,   c o n s t   w c h a r _ t *   c h a n g e ,   c o n s t   w c h a r _ t *   t o ) {  
         A T L : : C S t r i n g W   t m p ( s o u r c e . c _ s t r ( ) ) ;  
         t m p . R e p l a c e ( c h a n g e , t o ) ;  
         s o u r c e   =   t m p ;  
         r e t u r n   t r u e ;  
     }  
  
     b o o l   s t r r e p l a c e ( s t r i n g &   s o u r c e ,   c o n s t   c h a r *   c h a n g e ,   c o n s t   c h a r *   t o ) {  
         A T L : : C S t r i n g A   t m p ( s o u r c e . c _ s t r ( ) ) ;  
         t m p . R e p l a c e ( c h a n g e , t o ) ;  
         s o u r c e   =   t m p ;  
         r e t u r n   t r u e ;  
     }  
  
     s t r i n g   G e t F i l e E x t ( c o n s t   s t r i n g &   s t r , c h a r   d e l i m i t e r ) {  
         s i z e _ t   t 1   =   s t r . f i n d _ l a s t _ o f ( d e l i m i t e r ) ;  
         r e t u r n   s t r . s u b s t r ( t 1 + 1 , s t r . s i z e ( ) - t 1 - 1 ) ;  
     }  
  
     w s t r i n g   G e t F i l e E x t ( c o n s t   w s t r i n g &   s t r , w c h a r _ t   d e l i m i t e r ) {  
         s i z e _ t   t 1   =   s t r . f i n d _ l a s t _ o f ( d e l i m i t e r ) ;  
         r e t u r n   s t r . s u b s t r ( t 1 + 1 , s t r . s i z e ( ) - t 1 - 1 ) ;  
     }  
  
     v o i d   F i l t e r S t r ( w s t r i n g &   s t r ) {  
         s t a t i c   c o n s t   w c h a r _ t *   c o n s t   k f i l t e r   =   L " ] [ ` | { } / \ \ ~ ! @ # $ % & * ( ) + = ' \ " : ; ? < > �{  
         i f   ( o r i n a m e = = N U L L ) {  
             r e t u r n   G e n U n i q S t r 2 ( ) ;  
         }  
         w s t r i n g   t m p ( o r i n a m e ) ;  
         F i l t e r S t r ( t m p ) ;  
         s t r i n g   t m p 2   =   s t r i n g m a k e r ( C P _ A C P )   < <   t m p ;  
         P c r e   r e 2 ( s t r i n g ( " [ \ \ s ] + " ) , P C R E _ C A S E L E S S ) ;  
         s t r i n g   t m p 3   =   r e 2 . r e p l a c e ( t m p 2 , s t r i n g ( "   " ) ) ;  
         r e t u r n   w s t r i n g m a k e r ( C P _ A C P )   < <   t m p 3 ;  
     }  
  
     w s t r i n g   G e t S u g T x t ( c o n s t   w c h a r _ t *   s ) {  
         s t a t i c   c o n s t   c h a r *   c o n s t   k s t o p w o r d s   =    
             " , ( ( a ) | ( a n ) | ( a n d ) | ( a r e ) | ( a s ) | ( a t ) | ( b e ) | ( b u t ) | ( b y ) | ( f o r ) | ( i f ) | ( i n ) | ( i n t o ) | ( i s ) | ( i t ) | ( n o ) | ( n o t ) | ( o f ) | "  
             " ( o n ) | ( o r ) | ( s u c h ) | ( t h a t ) | ( t h e ) | ( t h e i r ) | ( t h e n ) | ( t h e r e ) | ( t h e s e ) | ( t h e y ) | ( t h i s ) | ( t o ) | ( w a s ) | ( w i l l ) | ( w i t h ) ) , " ;  
         w s t r i n g   t m p ( s ) ;  
         F i l t e r S t r ( t m p ) ;  
         s t r i n g   t m p 2   =   s t r i n g m a k e r ( C P _ A C P )   < <   t m p ;  
         P c r e   r e 2 ( s t r i n g ( " [ \ \ s ] + " ) , P C R E _ C A S E L E S S ) ;  
         s t r i n g   t m p 3   =   r e 2 . r e p l a c e ( t m p 2 , s t r i n g ( " , " ) ) ;  
         P c r e   r e 3 ( s t r i n g ( k s t o p w o r d s ) , P C R E _ C A S E L E S S ) ;  
         s t r i n g   t m p 4   =   r e 3 . r e p l a c e ( t m p 3 , s t r i n g ( " , " ) ) ;  
         r e t u r n   w s t r i n g m a k e r ( C P _ A C P )   < <   t m p 4 ;  
     }  
 # e n d i f  
  
     w c h a r _ t *   t o W i d e S t r i n g ( c o n s t   c h a r   * m b s t r , u n s i g n e d   i n t   c o d e P a g e ) {  
         w c h a r _ t   * w s t r   =   0 ;  
         i f   ( m b s t r )   {  
             s i z e _ t   o r i g s i z e   =   s t r l e n ( m b s t r ) ;  
             i f   ( o r i g s i z e   = =   0 )   {  
                 w s t r   =   n e w   w c h a r _ t   [ 1 ] ;  
                 w s t r   [ 0 ]   =   0 ;  
                 r e t u r n   w s t r ;  
             }  
             i n t   r c   =   M u l t i B y t e T o W i d e C h a r ( c o d e P a g e ,   0 ,   m b s t r ,   - 1 ,   w s t r ,   0 ) ;  
             w s t r   =   n e w   w c h a r _ t   [ r c + 1 ] ;  
             r c   =   M u l t i B y t e T o W i d e C h a r ( c o d e P a g e ,   0 ,   m b s t r ,   - 1 ,   w s t r ,   r c ) ;  
         }  
         r e t u r n   w s t r ;  
     }  
  
     b o o l   t o W i d e S t r i n g ( c o n s t   c h a r   * m b s t r , w s t r i n g &   d e s t , u n s i g n e d   i n t   c o d e P a g e ) {  
         t r y {  
             w c h a r _ t *   w s t r   =   t o W i d e S t r i n g ( m b s t r ,   c o d e P a g e ) ;  
             d e s t   =   w s t r ;  
             d e l e t e   [ ]   w s t r ;  
             r e t u r n   t r u e ;  
         } c a t c h ( . . . ) {  
             r e t u r n   f a l s e ;  
         }  
     }  
  
     c h a r *   t o A n s i S t r i n g ( c o n s t   w c h a r _ t   * w c s , u n s i g n e d   i n t   c o d e P a g e ) {  
         c h a r   * m b s t r   =   0 ;  
         i f   ( w c s )   {  
             s i z e _ t   n u m C o n v e r t e d C h a r s   =   0 ;  
             s i z e _ t   o r i g s i z e   =   w c s l e n ( w c s )   + 1 ;  
             i n t   r c   =   W i d e C h a r T o M u l t i B y t e ( c o d e P a g e ,   0 ,   w c s ,   - 1 ,   m b s t r ,   0 ,   N U L L ,   N U L L ) ;  
             m b s t r   =   n e w   c h a r   [ r c + 1 ] ;  
             W i d e C h a r T o M u l t i B y t e ( c o d e P a g e ,   0 ,   w c s ,   - 1 ,   m b s t r ,   r c ,   N U L L ,   N U L L ) ;  
             m b s t r [ r c ]   =   0 ;  
         }  
         r e t u r n   m b s t r ;  
     }  
  
     b o o l   t o A n s i S t r i n g ( c o n s t   w c h a r _ t   * w c s , s t r i n g &   d e s t , u n s i g n e d   i n t   c o d e P a g e ) {  
         t r y {  
             c h a r *   s t r   =   t o A n s i S t r i n g ( w c s ,   c o d e P a g e ) ;  
             d e s t   =   s t r ;  
             d e l e t e   [ ]   s t r ;  
             r e t u r n   t r u e ;  
         } c a t c h ( . . . ) {  
             r e t u r n   f a l s e ;  
         }  
     }  
  
     w s t r i n g m a k e r &   w s t r i n g m a k e r : : o p e r a t o r < < ( D W O R D   d w N u m ) {  
         D W O R D   d w T e m p   =   d w N u m ;  
         i n t   i C n t = 1 ;   / /   n a m e   l o o k u p   o f   ' i C n t '   c h a n g e d   f o r   n e w   I S O   ' f o r '   s c o p i n g  
         f o r (   ;   ( d w T e m p / = 1 0 )   ! =   0 ;   i C n t + +   ) ;  
         s i z e _ t   n s i z e   =   s t r _ . s i z e ( )   +   i C n t ;  
         s t r _ . r e s i z e ( n s i z e ) ;  
         c o n s t   w c h a r _ t *   t m p   =   s t r _ . c _ s t r ( ) ;  
         w c h a r _ t *   b u f f e r   =   & ( * s t r _ . b e g i n ( ) ) ;  
         i f ( ! S U C C E E D E D ( S t r i n g C c h P r i n t f W ( b u f f e r , n s i z e + 1 , L " % s % u " , t m p , d w N u m ) ) ) {  
             s t r _   =   L " " ;  
         }  
         r e t u r n   * t h i s ;  
     }  
  
     w s t r i n g m a k e r &   w s t r i n g m a k e r : : o p e r a t o r < < ( c o n s t   w s t r i n g &   s t r a d d ) {  
         s t r _   + =   s t r a d d ;  
         r e t u r n   * t h i s ;  
     }  
  
     w s t r i n g m a k e r &   w s t r i n g m a k e r : : o p e r a t o r < < ( c o n s t   s t r i n g &   s t r a d d ) {  
         w s t r i n g   t m p ;  
         t o W i d e S t r i n g ( s t r a d d . c _ s t r ( ) , t m p , c o d e p a g e _ ) ;  
         s t r _   + =   t m p ;  
         r e t u r n   * t h i s ;  
     }  
  
     s t r i n g m a k e r &   s t r i n g m a k e r : : o p e r a t o r < < ( D W O R D   d w N u m ) {  
         D W O R D   d w T e m p   =   d w N u m ;  
         i n t   i C n t = 1 ;   / /   n a m e   l o o k u p   o f   ' i C n t '   c h a n g e d   f o r   n e w   I S O   ' f o r '   s c o p i n g  
         f o r (   ;   ( d w T e m p / = 1 0 )   ! =   0 ;   i C n t + +   ) ;  
         s i z e _ t   n s i z e   =   s t r _ . s i z e ( )   +   i C n t ;  
         s t r _ . r e s i z e ( n s i z e ) ;  
         c o n s t   c h a r *   t m p   =   s t r _ . c _ s t r ( ) ;  
         c h a r *   b u f f e r   =   & ( * s t r _ . b e g i n ( ) ) ;  
         i f ( ! S U C C E E D E D ( S t r i n g C c h P r i n t f A ( b u f f e r , n s i z e + 1 , " % s % u " , t m p , d w N u m ) ) ) {  
             s t r _   =   " " ;  
         }  
         r e t u r n   * t h i s ;  
     }  
  
     s t r i n g m a k e r &   s t r i n g m a k e r : : o p e r a t o r < < ( c o n s t   s t r i n g &   s t r a d d ) {  
         s t r _   + =   s t r a d d ;  
         r e t u r n   * t h i s ;  
     }  
  
     s t r i n g m a k e r &   s t r i n g m a k e r : : o p e r a t o r < < ( c o n s t   w s t r i n g &   s t r a d d ) {  
         s t r i n g   t m p ;  
         t o A n s i S t r i n g ( s t r a d d . c _ s t r ( ) , t m p , c o d e p a g e _ ) ;  
         s t r _   + =   t m p ;  
         r e t u r n   * t h i s ;  
     }  
  
 } ; 