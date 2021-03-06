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

��# d e f i n e   M Y 6 B E E C O M M O N D _ A P I   _ _ d e c l s p e c ( d l l e x p o r t )  
  
 # i n c l u d e   " 6 b e e c o m m o n d . h "  
 # i n c l u d e   < c o m u t i l . h > / / _ v a r i a n t _ t  
 # i n c l u d e   < a t l a p p . h >  
 # i n c l u d e   < a t l s t r . h >  
 # i n c l u d e   < a t l m i s c . h >  
 # i n c l u d e   < a t l g d i . h >  
 # i n c l u d e   < u x t h e m e . h >  
 # i n c l u d e   " . . / . . / l i b / 6 b e e b a s e / c o p y d a t a m s g . h "  
 # i n c l u d e   " . . / . . / l i b / 6 b e e b a s e / c p p s q l i t e 3 . h "  
 # i n c l u d e   " . . / . . / l i b / 6 b e e b a s e / 6 b e e s _ u t i l . h "  
 # i n c l u d e   " . . / . . / l i b / 6 b e e b a s e / 6 b e e s _ n e t . h "  
 # i n c l u d e   " . . / . . / l i b / 6 b e e b a s e / 6 b e e s _ s o c k e t . h "  
 # i n c l u d e   " . . / . . / l i b / 6 b e e b a s e / 6 b e e s _ w i n d o w . h "  
 # i n c l u d e   " . . / . . / l i b / 6 b e e b a s e / 6 b e e s _ f t p c l i e n t . h "  
 # i n c l u d e   " . . / . . / l i b / 6 b e e b a s e / u u l o g g i n g . h "  
 # i n c l u d e   " . . / . . / l i b / 6 b e e b a s e / i c e s i n g l e t o n . h "  
 # i n c l u d e   " . . / . . / l i b / 6 b e e b a s e / 6 b e e s _ s t r . h "  
 # i n c l u d e   " . . / . . / l i b / 6 b e e b a s e / 6 b e e s _ i w e b b r o w s e r 2 . h " / / I W e b B r o w s e r 2 P t r  
 # i n c l u d e   < s h e l l a p i . h >  
 u s i n g   _ 6 b e e s _ s t r : : s t r i n g m a k e r ;  
 u s i n g   _ 6 b e e s _ s t r : : w s t r i n g m a k e r ;  
  
 # p r a g m a   c o m m e n t ( l i b , " v e r s i o n . l i b " ) / / G e t F i l e V e r s i o n  
  
 # p r a g m a   d a t a _ s e g ( " . s h a r e d " )  
 _ 6 b e e d _ s h a r e : : g c o n f   g l o b a l c o n f i g ( 0 ) ;  
 # p r a g m a   d a t a _ s e g ( )  
 # p r a g m a   c o m m e n t ( l i n k e r , " / S E C T I O N : . s h a r e d , R W S " )  
  
 B O O L   A P I E N T R Y   D l l M a i n ( H M O D U L E   h M o d u l e , D W O R D     u l _ r e a s o n _ f o r _ c a l l , L P V O I D   l p R e s e r v e d ) {  
     s w i t c h   ( u l _ r e a s o n _ f o r _ c a l l ) {  
     c a s e   D L L _ P R O C E S S _ A T T A C H :  
     c a s e   D L L _ T H R E A D _ A T T A C H :  
     c a s e   D L L _ T H R E A D _ D E T A C H :  
     c a s e   D L L _ P R O C E S S _ D E T A C H :  
         b r e a k ;  
     }  
     r e t u r n   T R U E ;  
 }  
  
 n a m e s p a c e   _ 6 b e e d _ u t i l {  
     u s i n g   n a m e s p a c e   _ 6 b e e s _ c o n s t ;  
  
     U I N T   W I N A P I   r u n I E _ i n t e r n a l ( P V O I D   p ) {  
         t r y {  
             _ v a r i a n t _ t   v a r U R L   =   ( w c h a r _ t * ) p ;  
             C o I n i t i a l i z e ( N U L L ) ;  
             I W e b B r o w s e r 2 P t r   p I E ( 0 ) ;  
             i f   ( S U C C E E D E D ( C o C r e a t e I n s t a n c e ( C L S I D _ I n t e r n e t E x p l o r e r , N U L L , C L S C T X _ L O C A L _ S E R V E R ,  
                 I I D _ I W e b B r o w s e r 2 , ( v o i d * * ) & p I E ) ) )  
             {  
                 p I E - > p u t _ V i s i b l e ( V A R I A N T _ T R U E ) ;  
                 _ v a r i a n t _ t   v a r E m p t y ;  
                 p I E - > N a v i g a t e 2 ( & v a r U R L , & v a r E m p t y , & v a r E m p t y , & v a r E m p t y , & v a r E m p t y ) ;  
             }  
             C o U n i n i t i a l i z e ( ) ;  
         } c a t c h   ( . . . ) { }  
         r e t u r n   0 ;  
     }  
  
     w s t r i n g   r u n I E _ u r l ;  
     v o i d   r u n I E ( c o n s t   w c h a r _ t *   u r l ) {  
         r u n I E _ u r l   =   u r l ;  
         : : _ b e g i n t h r e a d e x ( N U L L , 0 , r u n I E _ i n t e r n a l , ( L P V O I D ) r u n I E _ u r l . c _ s t r ( ) , 0 , 0 ) ;  
     }  
  
     b o o l   u r l e n c o d e ( c h a r *   b u f , i n t   b u f s i z e , c o n s t   c h a r *   s t r ) {  
         s t a t i c   c o n s t   c h a r   s p e c i a l [ ]   =   " ! \ " # $ % & ( ) * + , / : ; < = > ? @ [ \ \ ] ^ ` { | } ~ % " ;  
         c h a r   t m p [ 1 0 ] ; / *   t e m p   b u f f e r   * /    
         b u f [ 0 ]   =   ' \ 0 ' ;  
         i n t   j = ( i n t ) s t r l e n ( s t r ) ;  
         i f   ( j > 1 0 2 4 * 2 ) {  
             r e t u r n   f a l s e ;  
         }  
         f o r ( i n t   i = 0 ;   i < j ;   i + + ) {    
             c h a r   c   =   s t r [ i ] ;    
             i f ( s t r c h r ( s p e c i a l , c ) ) {   / *   s p e c i a l   c h a r a c t e r   * /    
                 s p r i n t f _ s ( t m p , " % % % 0 2 X " , ( u n s i g n e d   c h a r ) c ) ;    
                 s t r c a t _ s ( b u f , b u f s i z e , t m p ) ;    
             } e l s e   i f ( c   = =   '   ' ) {   / *   b l a n k   * /    
                 s t r c a t _ s ( b u f , b u f s i z e , " + " ) ;  
             } e l s e {    
                 i f ( c   <   0 ) {   / *   n o n e   A S C I I   c h a r a c t e r   * /  
                     s p r i n t f _ s ( t m p , " % % % 0 2 X % % % 0 2 X " , ( u n s i g n e d   c h a r ) s t r [ i ] , ( u n s i g n e d   c h a r ) s t r [ i + 1 ] ) ;    
                     s t r c a t _ s ( b u f , b u f s i z e , t m p ) ;    
                     + + i ;    
                 } e l s e {   / *   A S C I I   c h a r a c t e r   * /    
                     s p r i n t f _ s ( t m p , " % c " , c ) ;    
                     s t r c a t _ s ( b u f , b u f s i z e , t m p ) ;    
                 }    
             }    
         }  
         r e t u r n   t r u e ;  
     }  
  
     b o o l   u r l d e c o d e _ g b 2 3 1 2 ( c h a r   * d e s t , i n t   d e s t s i z e , c o n s t   c h a r *   s r c ) {  
         : : m e m s e t ( d e s t , ' \ 0 ' , d e s t s i z e ) ;  
         c o n s t   c h a r   * p   =   s r c ;  
         c h a r   c o d e [ 3 ]   =   { 0 } ;  
         u n s i g n e d   l o n g   a s c i i   =   0 ;  
         c h a r   * e n d   =   N U L L ;  
         w h i l e ( * p ) {  
             i f ( * p   = =   ' % ' ) {  
                 : : m e m c p y ( c o d e , + + p , 2 ) ;  
                 a s c i i   =   s t r t o u l ( c o d e , & e n d , 1 6 ) ;  
                 * d e s t + +   =   ( c h a r ) a s c i i ;  
                 p   + =   2 ;  
             } e l s e  
                 * d e s t + +   =   * p + + ;  
         }  
         r e t u r n   t r u e ;  
     }  
  
     b o o l   G e t F i l e S i z e A ( c o n s t   c h a r *   f p a t h , i n t &   f s i z e ) {  
         W I N 3 2 _ F I L E _ A T T R I B U T E _ D A T A     f i l e a t t r ;  
         i f   ( : : G e t F i l e A t t r i b u t e s E x A ( ( L P C S T R ) f p a t h , : : G e t F i l e E x I n f o S t a n d a r d , & f i l e a t t r ) ) {  
             f s i z e   =   ( f i l e a t t r . n F i l e S i z e H i g h   *   ( M A X W O R D + 1 ) )   +   f i l e a t t r . n F i l e S i z e L o w ;  
             r e t u r n   t r u e ;  
         }  
         r e t u r n   f a l s e ;  
     }  
  
     b o o l   G e t F i l e S i z e W ( c o n s t   w c h a r _ t *   f p a t h , i n t &   f s i z e ) {  
         W I N 3 2 _ F I L E _ A T T R I B U T E _ D A T A     f i l e a t t r ;  
         i f   ( : : G e t F i l e A t t r i b u t e s E x W ( ( L P C T S T R ) f p a t h , : : G e t F i l e E x I n f o S t a n d a r d , & f i l e a t t r ) ) {  
             f s i z e   =   ( f i l e a t t r . n F i l e S i z e H i g h   *   ( M A X W O R D + 1 ) )   +   f i l e a t t r . n F i l e S i z e L o w ;  
             r e t u r n   t r u e ;  
         }  
         r e t u r n   f a l s e ;  
     }  
  
     b o o l   G e t 6 B e e L o c a t i o n ( w c h a r _ t *   p s z P a t h B u f f e r , i n t   b u f s i z e = M A X _ P A T H ) {  
         D W O R D   d w R e t V a l = M A X _ P A T H ;  
         H K E Y   h k e y = N U L L ;  
         b o o l   r e s u l t   =   f a l s e ;  
         : : w m e m s e t ( p s z P a t h B u f f e r , L ' \ 0 ' , b u f s i z e ) ;  
         i f ( R e g O p e n K e y ( H K E Y _ L O C A L _ M A C H I N E , k r e g i s t r y n a m e , & h k e y )   = =   E R R O R _ S U C C E S S ) {  
             D W O R D   d w T y p e   =   R E G _ E X P A N D _ S Z ;  
             w c h a r _ t   s z P a t h [   M A X _ P A T H   ] = { } ;  
             i f ( R e g Q u e r y V a l u e E x ( h k e y , _ T ( " L o c a t i o n " ) , 0 , & d w T y p e , ( L P B Y T E ) s z P a t h , & d w R e t V a l )   = =   E R R O R _ S U C C E S S ) {  
                 d w R e t V a l   =   E x p a n d E n v i r o n m e n t S t r i n g s ( s z P a t h , p s z P a t h B u f f e r , M A X _ P A T H ) ;  
                 i f ( ! h k e y ) R e g C l o s e K e y ( h k e y ) ;  
                 r e s u l t   =   t r u e ;  
             }  
         }  
         i f   ( ! r e s u l t ) {  
             _ 6 b e e s _ u t i l : : q u i t f a t a l e r r o r (  
                 " Y o u r   r e g i s t r y   i s   c o r r u p t e d . \ n Y o u   c a n   s o l v e   t h i s   p r o b l e m   b y   r e i n s t a l l   u l t r a i e . " ) ;  
         }  
         r e t u r n   r e s u l t ;  
     }  
  
     c o n s t   w c h a r _ t *   G e t 6 B E E P a t h ( c o n s t   w c h a r _ t *   s o u r c e , b o o l   a p p e n d s l a s h ) {  
         s t a t i c   w c h a r _ t   t m p [ M A X _ P A T H ] ;  
         G e t 6 B e e L o c a t i o n ( t m p ) ;  
         i f ( s o u r c e = = 0 )  
             r e t u r n   t m p ;  
         : : w c s c a t _ s ( t m p , M A X _ P A T H , s o u r c e ) ;  
         i f ( a p p e n d s l a s h )  
             : : w c s c a t _ s ( t m p , M A X _ P A T H , _ T ( " \ \ " ) ) ;  
         r e t u r n   t m p ;  
     }  
  
     / / /   @ t o d o   r e m o v e   s t a t i c   f o r   m u l t i t h r e a d   c o n s i d e r a t i o n  
     c o n s t   c h a r *   G e t 6 B E E P a t h A ( c o n s t   w c h a r _ t *   s o u r c e , b o o l   a p p e n d s l a s h ) {  
         U S E S _ C O N V E R S I O N ;  
         w c h a r _ t   t m p [ M A X _ P A T H ] = { } ;  
         i f ( G e t 6 B e e L o c a t i o n ( t m p ) ) {  
             i f ( s o u r c e = = 0 ) {  
                 c o n s t   c h a r   * p   =   W 2 C A ( t m p ) ;  
                 s t a t i c   c h a r   t m p A [ M A X _ P A T H ] ;  
                 s t r c p y _ s ( t m p A , M A X _ P A T H , p ) ;  
                 r e t u r n   t m p A ;  
             }  
             : : w c s c a t _ s ( t m p , M A X _ P A T H , s o u r c e ) ;  
             i f ( a p p e n d s l a s h )  
                 : : w c s c a t _ s ( t m p , M A X _ P A T H , _ T ( " \ \ " ) ) ;  
  
             c o n s t   c h a r   * p   =   W 2 C A ( t m p ) ;  
             s t a t i c   c h a r   t m p A [ M A X _ P A T H ] ;  
             s t r c p y _ s ( t m p A , M A X _ P A T H , p ) ;  
             r e t u r n   t m p A ;  
         }  
         r e t u r n   N U L L ;  
     }  
  
     v o i d   S e n d M s g T o U p l o a d e r ( H W N D   h w n d T o , c o n s t   c h a r *   f i l e l i s t , i n t   m s g ) {  
         i n t   s l e n   =   ( i n t ) s t r l e n ( f i l e l i s t ) ;  
         : : m e m s e t ( ( v o i d * ) ( f i l e l i s t + s l e n ) , ' \ 0 ' , 1 ) ;  
         C O P Y D A T A S T R U C T   c d s ;  
         c d s . d w D a t a   =   m s g ;  
         c d s . l p D a t a   =   ( P V O I D ) f i l e l i s t ;  
         c d s . c b D a t a   =   s l e n + 1 ;  
         : : S e n d M e s s a g e ( h w n d T o , W M _ C O P Y D A T A , ( W P A R A M ) 0 , ( L P A R A M ) & c d s ) ;  
     }  
  
     b o o l   g e t t i m e s t r ( c h a r *   t s t r , i n t   l e n ) {  
         i f ( l e n < 5 0 )   r e t u r n   f a l s e ;  
         t r y {  
             t i m e _ t   r a w t i m e = 0 ;  
             t i m e ( & r a w t i m e ) ;  
             s t a t i c   c h a r   t [ 5 0 ] = { 0 } ;  
             c t i m e _ s ( t , 5 0 , & r a w t i m e ) ;  
             A T L : : C S t r i n g A   c t ( t ) ;  
             c t . T r i m R i g h t ( ) ;  
             s t r c p y _ s ( t s t r , l e n , c t . G e t S t r i n g ( ) ) ;  
         } c a t c h ( . . . ) {  
             r e t u r n   f a l s e ;  
         }  
         r e t u r n   t r u e ;  
     }  
  
  
     c h a r *   G e t I E H o m e P a g e ( ) {  
         s t a t i c   c h a r   s z l p D a t a [ M A X _ P A T H + 1 ] = { } ;  
         H K E Y   h K e y ;  
         D W O R D   l R e s u l t   =   R e g O p e n K e y E x A ( H K E Y _ C U R R E N T _ U S E R , " S o f t w a r e \ \ M i c r o s o f t \ \ I n t e r n e t   E x p l o r e r \ \ M a i n \ \ " , 0 , K E Y _ A L L _ A C C E S S , & h K e y ) ;  
         i f   ( l R e s u l t   = =   E R R O R _ S U C C E S S ) {  
             D W O R D     c b D a t a   =   M A X _ P A T H , d w T y p e   =   R E G _ S Z ;  
             : : R e g Q u e r y V a l u e E x A ( h K e y , " S t a r t   P a g e " , N U L L , & d w T y p e , ( L P B Y T E ) s z l p D a t a , & c b D a t a ) ;  
             R e g C l o s e K e y ( h K e y ) ;  
         }  
         r e t u r n   s z l p D a t a ;  
     }  
     w c h a r _ t *   G e t I E H o m e P a g e W ( ) {  
         U S E S _ C O N V E R S I O N ;  
         r e t u r n   A 2 W ( G e t I E H o m e P a g e ( ) ) ;  
     }  
  
     O S T   G e t O S ( ) {  
         O S V E R S I O N I N F O   o s v i = { } ;  
         o s v i . d w O S V e r s i o n I n f o S i z e   =   s i z e o f ( O S V E R S I O N I N F O ) ;  
         : : G e t V e r s i o n E x   ( & o s v i ) ;  
         i f ( o s v i . d w P l a t f o r m I d = = V E R _ P L A T F O R M _ W I N 3 2 _ N T ) {  
             i f ( ( o s v i . d w M a j o r V e r s i o n = = 5 ) & & ( o s v i . d w M i n o r V e r s i o n > = 1 ) ) {  
                 r e t u r n   X P ;  
             } e l s e   i f ( o s v i . d w M a j o r V e r s i o n > = 6 ) {  
                 r e t u r n   V I S T A ;  
             } e l s e {  
                 r e t u r n   O T H E R N T ;  
             }  
         } e l s e {  
             r e t u r n   O L D W I N ;  
         }  
     }  
  
     b o o l   I s X P T h e m e ( ) {  
         t y p e d e f   H T H E M E   ( _ _ s t d c a l l   *   P O P E N T H E M E D A T A ) ( H W N D   h w n d , L P C W S T R   p s z C l a s s L i s t ) ;  
         t y p e d e f   H R E S U L T   ( _ _ s t d c a l l   *   P C L O S E T H E M E D A T A ) ( H T H E M E   h T h e m e ) ;  
         O S T   o s T y p e = G e t O S ( ) ;  
         i f   ( o s T y p e ! = X P   & &   o s T y p e ! = V I S T A ) {  
             r e t u r n   f a l s e ;  
         }  
         b o o l   b X p V i s u a l S t y l e = f a l s e ;  
         H M O D U L E   h U x T h e m e = : : L o a d L i b r a r y ( _ T ( " U x T h e m e . d l l " ) ) ;  
         i f ( h U x T h e m e ) {  
             P O P E N T H E M E D A T A   p O p e n T h e m e D a t a = ( P O P E N T H E M E D A T A ) : : G e t P r o c A d d r e s s ( h U x T h e m e , " O p e n T h e m e D a t a " ) ;  
             P C L O S E T H E M E D A T A   p C l o s e T h e m e D a t a = ( P C L O S E T H E M E D A T A ) : : G e t P r o c A d d r e s s   ( h U x T h e m e , " C l o s e T h e m e D a t a " ) ;  
             H T H E M E   h B u t t o n T h e m e = ( * p O p e n T h e m e D a t a ) ( G e t D e s k t o p W i n d o w ( ) , L " B U T T O N " ) ;  
             b X p V i s u a l S t y l e = ( h B u t t o n T h e m e ! = N U L L ) ;  
             ( * p C l o s e T h e m e D a t a ) ( h B u t t o n T h e m e ) ;  
         }  
         : : F r e e L i b r a r y ( h U x T h e m e ) ;  
         r e t u r n   b X p V i s u a l S t y l e ;  
     }  
  
     b o o l   i s C h i n e s e O S ( ) {  
         r e t u r n   G e t S y s t e m D e f a u l t L a n g I D ( ) = = 0 x 8 0 4 ;  
     }  
  
     c o n s t   c h a r *   g e t L a n g E n c o d i n g ( ) {  
         s w i t c h   ( G e t S y s t e m D e f a u l t L a n g I D ( ) ) {  
     c a s e   0 x 8 0 4 :  
         r e t u r n   " G B 2 3 1 2 " ;  
     d e f a u l t :  
         r e t u r n   " U T F - 8 " ;  
         }  
     }  
  
     v o i d   C a l l E x e W ( c o n s t   w c h a r _ t *   c ) {  
         S T A R T U P I N F O W   s i   =   { s i z e o f ( s i ) } ;  
         P R O C E S S _ I N F O R M A T I O N   p i ;  
         i f   ( C r e a t e P r o c e s s W ( N U L L , c o n s t _ c a s t < w c h a r _ t * > ( c ) , N U L L , N U L L , f a l s e , 0 , N U L L , N U L L , & s i , & p i ) ) {  
             C l o s e H a n d l e ( p i . h P r o c e s s ) ;  
             C l o s e H a n d l e ( p i . h T h r e a d ) ;  
         } e l s e {  
             : : M e s s a g e B o x W ( N U L L , c , L " E R R O R :   F a i l e d   t o   l a u n c h . " , M B _ I C O N I N F O R M A T I O N ) ;  
         }  
     }  
  
     v o i d   C a l l E x e A ( c o n s t   c h a r *   c ) {  
         S T A R T U P I N F O A   s i   =   { s i z e o f ( s i ) } ;  
         P R O C E S S _ I N F O R M A T I O N   p i ;  
         i f   ( C r e a t e P r o c e s s A ( N U L L , c o n s t _ c a s t < c h a r * > ( c ) , N U L L , N U L L , f a l s e , 0 , N U L L , N U L L , & s i , & p i ) ) {  
             C l o s e H a n d l e ( p i . h P r o c e s s ) ;  
             C l o s e H a n d l e ( p i . h T h r e a d ) ;  
         } e l s e {  
             : : M e s s a g e B o x A ( N U L L , c , " E R R O R :   F a i l e d   t o   l a u n c h . " , M B _ I C O N I N F O R M A T I O N ) ;  
         }  
     }  
  
     b o o l   S e t M M F A ( c o n s t   c h a r *   n a m e , c o n s t   c h a r *   c o n t e n t s , i n t   b u f s i z e ) {  
 # i f d e f   _ D E B U G  
         U U D E B U G ( ( L O G _ E R R O R , " - - - - - - - - - - - - -   S e t M M F   - - - - - - - - - - - - - - " ) ) ;  
 # e n d i f  
         H A N D L E   s _ h F i l e M a p   =   C r e a t e F i l e M a p p i n g A ( I N V A L I D _ H A N D L E _ V A L U E , N U L L , P A G E _ R E A D W R I T E , 0 , b u f s i z e , n a m e ) ;  
         i f   ( s _ h F i l e M a p ! = N U L L ) {  
                 i f   ( G e t L a s t E r r o r ( ) = = E R R O R _ A L R E A D Y _ E X I S T S ) {  
                     / /   m a p p i n g   a l r e a d y   e x i s t s  
                     C l o s e H a n d l e ( s _ h F i l e M a p ) ;  
                 } e l s e {  
                     c h a r *   p V i e w   =   r e i n t e r p r e t _ c a s t < c h a r   * > ( M a p V i e w O f F i l e ( s _ h F i l e M a p , F I L E _ M A P _ R E A D | F I L E _ M A P _ W R I T E , 0 , 0 , 0 ) ) ;  
                     i f   ( p V i e w ! = N U L L ) {  
                         / / w c s c p y _ s ( ( w c h a r _ t * ) p V i e w , b u f s i z e , c o n t e n t s ) ; / / /   @ t o d o     -   t o   c h e c k   s h o u l d   i t   b e   b u f s i z e / 2 ? ? ?  
                         s t r i n g   c ( c o n t e n t s ) ;  
                         i f   ( c . s i z e ( ) > M M F _ S I Z E - 1 ) {  
                             c   =   c . s u b s t r ( 0 , M M F _ S I Z E - 1 ) ;  
                         }  
                         m e m s e t ( p V i e w , ' \ 0 ' , b u f s i z e ) ;  
                         m e m c p y ( p V i e w , c . c _ s t r ( ) , c . s i z e ( ) + 1 ) ;  
 # i f d e f   _ D E B U G  
                         U U D E B U G ( ( L O G _ E R R O R , n a m e ) ) ;  
                         U U D E B U G ( ( L O G _ E R R O R , c . c _ s t r ( ) ) ) ;  
 # e n d i f  
                         U n m a p V i e w O f F i l e ( p V i e w ) ;  
                     } e l s e {  
                         / /   c a n n o t   m a p   v i e w   o f   f i l e  
                     }  
                 }  
         } e l s e {  
             / /   c a n n o t   o p e n   m a p p i n g  
         }  
         r e t u r n   t r u e ;  
     }  
  
     b o o l   S e t M M F ( c o n s t   w c h a r _ t *   n , c o n s t   w c h a r _ t *   c , i n t   b u f s i z e ) {  
         s t r i n g   n a m e   =   s t r i n g m a k e r ( )   < <   n ;  
         s t r i n g   c o n t e n t s   =   s t r i n g m a k e r ( )   < <   c ;  
         r e t u r n   S e t M M F A ( n a m e . c _ s t r ( ) , c o n t e n t s . c _ s t r ( ) , b u f s i z e ) ;  
     }  
  
     v o i d   I n v o k e U p l o a d e r A ( c o n s t   c h a r *   f i l e p a t h , c o n s t   c h a r *   d e s c , c o n s t   c h a r *   t a g , c o n s t   c h a r *   t h u m b p a t h ) {  
         H W N D   h w n d T o   =   : : F i n d W i n d o w ( 0 , k U p l o a d e r _ W n d T i t l e ) ;  
         s t r i n g   m m f n a m e _ d e s c , m m f n a m e _ t a g , m m f n a m e _ t h u m b ;  
         s t r i n g   f n a m e   =   P a t h F i n d F i l e N a m e A ( f i l e p a t h ) ;  
         _ 6 b e e s _ u t i l : : G e n M M F N a m e ( f n a m e , m m f n a m e _ d e s c , m m f n a m e _ t a g , m m f n a m e _ t h u m b ) ;  
         S e t M M F A ( m m f n a m e _ d e s c . c _ s t r ( ) , d e s c ) ;  
         S e t M M F A ( m m f n a m e _ t a g . c _ s t r ( ) , t a g ) ;  
         S e t M M F A ( m m f n a m e _ t h u m b . c _ s t r ( ) , t h u m b p a t h ) ;  
         i f ( ! h w n d T o ) {  
             u s i n g   _ 6 b e e s _ u t i l : : u l t r a p a t h ;  
             c o n s t   w s t r i n g &   u p a t h   =   u l t r a p a t h : : R ( ) . g e t ( u l t r a p a t h : : u p l o a d e r ) ;  
             w s t r i n g   U p l o a d e r C m d   =   w s t r i n g m a k e r ( C P _ A C P )    
                 < <   u p a t h  
                 < <   L "   - m   d i r e c t u p l o a d   - f   \ " "   < <   f i l e p a t h   < < L " \ " " ;  
             S T A R T U P I N F O   s i   =   { s i z e o f ( s i ) } ;  
             P R O C E S S _ I N F O R M A T I O N   p i ;  
             i f   ( C r e a t e P r o c e s s ( N U L L , ( L P W S T R ) U p l o a d e r C m d . c _ s t r ( ) , N U L L , N U L L , f a l s e , 0 , N U L L , N U L L , & s i , & p i ) ) {  
                 C l o s e H a n d l e ( p i . h P r o c e s s ) ;  
                 C l o s e H a n d l e ( p i . h T h r e a d ) ;  
             } e l s e {  
                 : : M e s s a g e B o x ( N U L L , _ T ( " C a n n o t   l a u n c h   U l t r a I E   U p l o a d e r ! " ) , _ T ( " E R R O R " ) , M B _ I C O N I N F O R M A T I O N ) ;  
             }  
         } e l s e {  
             s t r i n g   a f i l e p a t h = s t r i n g m a k e r ( C P _ A C P ) < < f i l e p a t h ;  
             S e n d M s g T o U p l o a d e r ( h w n d T o , a f i l e p a t h . c _ s t r ( ) , C P Y D A T A _ U P L O A D _ 2 _ U P L O A D _ D I R E C T ) ;  
         }  
     }  
  
     / / /   @ t o d o   f o r   e n g l i s h   O S ,   i f   p a r a m e t e r   i s   c h i n e s e ,   f u n n y   c h a r a c t e r   w i l l   c o m e   o u t ! ! !  
     / / /   I   c a n   n o t   w o r k   i t   o u t .   M a y b e   I   n e e d   t o   a v o i d   t h e   c o n v e r s i o n   f r o m   U n i c o d e   t o   M B C S .  
     v o i d   I n v o k e U p l o a d e r ( c o n s t   w c h a r _ t *   f i l e p a t h ,  
         c o n s t   w c h a r _ t *   d e s c ,  
         c o n s t   w c h a r _ t *   t a g ,  
         c o n s t   w c h a r _ t *   t h u m b p a t h )  
     {  
         s t r i n g   f p a t h _   =   s t r i n g m a k e r ( C P _ A C P )   < <   f i l e p a t h ;  
         s t r i n g   d e s c _   =   s t r i n g m a k e r ( C P _ A C P )   < <   d e s c ;  
         s t r i n g   t a g _   =   s t r i n g m a k e r ( C P _ A C P )   < <   t a g ;  
         s t r i n g   t h u m b p a t h _   =   s t r i n g m a k e r ( C P _ A C P )   < <   t h u m b p a t h ;  
         I n v o k e U p l o a d e r A ( f p a t h _ . c _ s t r ( ) , d e s c _ . c _ s t r ( ) , t a g _ . c _ s t r ( ) , t h u m b p a t h _ . c _ s t r ( ) ) ;  
     }  
  
     v o i d   D r a w G r a d i e n t ( H D C   h d c , L P R E C T   l p R e c t , C O L O R R E F   r g b T o p , C O L O R R E F   r g b B o t t o m ) {  
         G R A D I E N T _ R E C T   g r d R e c t   =   {   0 , 1   } ;  
         T R I V E R T E X   t r i V e r t e x t [   2   ]   =   {  
             l p R e c t - > l e f t ,  
             l p R e c t - > t o p ,  
             G e t R V a l u e ( r g b T o p )   < <   8 ,  
             G e t G V a l u e ( r g b T o p )   < <   8 ,  
             G e t B V a l u e ( r g b T o p )   < <   8 ,  
             0 x 0 0 0 0 ,  
             l p R e c t - > r i g h t ,  
             l p R e c t - > b o t t o m ,  
             G e t R V a l u e ( r g b B o t t o m )   < <   8 ,  
             G e t G V a l u e ( r g b B o t t o m )   < <   8 ,  
             G e t B V a l u e ( r g b B o t t o m )   < <   8 ,  
             0 x 0 0 0 0  
         } ;  
         : : G r a d i e n t F i l l ( h d c , t r i V e r t e x t , 2 , & g r d R e c t , 1 , G R A D I E N T _ F I L L _ R E C T _ V ) ;  
     }  
  
     v o i d   D r a w R o u n d R e c t ( H D C   h d c , L P R E C T   r c R e c t , C O L O R R E F   r g b O u t e r , C O L O R R E F   r g b I n n e r   , C O L O R R E F   m _ r g b B a c k g r o u n d ) {  
         C D C H a n d l e   c d c h a n d l e ( h d c ) ;  
         R E C T   t m p r e c t ;  
         : : C o p y R e c t ( & t m p r e c t , r c R e c t ) ;  
  
         C P e n   p e n B o r d e r ;  
         p e n B o r d e r . C r e a t e P e n ( P S _ S O L I D , 1 , r g b O u t e r ) ;  
         C B r u s h   b s h I n t e r i o r ;  
         b s h I n t e r i o r . C r e a t e S o l i d B r u s h ( m _ r g b B a c k g r o u n d ) ;  
  
         S e l e c t O b j e c t ( h d c , ( H G D I O B J ) p e n B o r d e r ) ;  
         S e l e c t O b j e c t ( h d c , ( H G D I O B J ) b s h I n t e r i o r ) ;  
  
         P O I N T   p = { } ;  
         c d c h a n d l e . R o u n d R e c t ( & t m p r e c t , p ) ;  
         : : I n f l a t e R e c t ( & t m p r e c t , - 1 , - 1 ) ;  
  
         C P e n   p e n I n n e r B o r d e r ;  
         p e n I n n e r B o r d e r . C r e a t e P e n ( P S _ S O L I D , 1 , r g b I n n e r ) ;  
         S e l e c t O b j e c t ( h d c , ( H G D I O B J ) p e n I n n e r B o r d e r ) ;  
  
         c d c h a n d l e . R o u n d R e c t ( & t m p r e c t , p ) ;  
     }  
  
     v e r s i o n b l k   G e t V e r s i o n B l k ( c o n s t   w c h a r _ t *   s F i l e N a m e ) {  
         v e r s i o n b l k   v e r ;  
         U I N T   j j   =   ( U I N T ) s i z e o f ( V S _ F I X E D F I L E I N F O ) ;  
         w c h a r _ t   * p p , p p 2 [ 1 0 0 ] ;  
         V S _ F I X E D F I L E I N F O   * l p B u f f e r 2 ;  
         v o i d   * * p p 1   =   ( v o i d   * * ) & l p B u f f e r 2 ;  
         p p 2 [ 0 ]   =   0 ;  
         i n t   i i   =   : : G e t F i l e V e r s i o n I n f o S i z e ( s F i l e N a m e , N U L L ) ;  
         i f ( i i > 0 ) {  
             p p   =   n e w   w c h a r _ t [ i i ] ;  
             i f ( : : G e t F i l e V e r s i o n I n f o ( s F i l e N a m e , 0 , i i , p p ) ) {  
                 i f ( : : V e r Q u e r y V a l u e ( p p , _ T ( " \ \ " ) , p p 1 , & j j ) ) {  
                     v e r . w M a j o r       =   H I W O R D ( l p B u f f e r 2 - > d w F i l e V e r s i o n M S ) ;  
                     v e r . w M i n o r       =   L O W O R D ( l p B u f f e r 2 - > d w F i l e V e r s i o n M S ) ;  
                     v e r . w R e l e a s e   =   H I W O R D ( l p B u f f e r 2 - > d w F i l e V e r s i o n L S ) ;  
                     v e r . w B u i l d       =   L O W O R D ( l p B u f f e r 2 - > d w F i l e V e r s i o n L S ) ;  
                 }  
             }  
             d e l e t e   [ ]   p p ;  
         }  
         r e t u r n   v e r ;  
     }  
  
     b o o l   L o g i n _ v 2 ( ) {  
         i f   ( ! _ 6 b e e s _ w i n d o w : : N o t i f y C l i e n t M n g ( C P Y D A T A _ L O G I N , k U p l o a d e r _ W n d T i t l e , N U L L ) ) {  
             S T A R T U P I N F O   s i   =   { s i z e o f ( s i ) } ;  
             P R O C E S S _ I N F O R M A T I O N   p i ;  
             u s i n g   _ 6 b e e s _ u t i l : : u l t r a p a t h ;  
             w s t r i n g   c l i e n t m n g p a t h ( u l t r a p a t h : : R ( ) . g e t ( u l t r a p a t h : : u p l o a d e r ) ) ;  
             c l i e n t m n g p a t h   + =   L "   - m   l o g i n " ;  
             i f   ( C r e a t e P r o c e s s ( N U L L , ( L P W S T R ) c l i e n t m n g p a t h . c _ s t r ( ) , N U L L , N U L L , f a l s e , 0 , N U L L , N U L L , & s i , & p i ) ) {  
                 C l o s e H a n d l e ( p i . h P r o c e s s ) ;  
                 C l o s e H a n d l e ( p i . h T h r e a d ) ;  
             } e l s e {  
                 : : M e s s a g e B o x ( N U L L , c l i e n t m n g p a t h . c _ s t r ( ) , _ T ( " E R R O R   W h e n   S t a r t i n g   L o g i n   P r o g r a m . " ) , M B _ I C O N I N F O R M A T I O N ) ;  
                 r e t u r n   f a l s e ;  
             }  
         }  
         r e t u r n   t r u e ;  
     }  
  
     b o o l   Q u i e t U p l o a d F i l e ( c o n s t   w c h a r _ t *   l o c a l n a m e ,  
         c o n s t   w c h a r _ t *   r e m o t e n a m e ,  
         c o n s t   w c h a r _ t *   i d ,  
         U P L O A D E R M O D E   m o d e )  
     {  
         i n t   m s i z e ;  
         i f ( ! G e t F i l e S i z e W ( l o c a l n a m e , m s i z e ) )  
             r e t u r n   f a l s e ;  
         i f ( m s i z e > 1 0 2 4 * 2 0 )  
             r e t u r n   f a l s e ;  
  
         _ 6 b e e s _ s o c k e t : : f t p c l i e n t   m y f t p C l i e n t ;  
         _ 6 b e e s _ t i m e r : : C T i m e r   m _ T i m e r ;  
         m y f t p C l i e n t . S e t T i m e r ( &   m _ T i m e r ) ;  
         i n t   s t a t i c   t m p i d = 0 ;  
         m y f t p C l i e n t . S e t T h r e a d I D ( 9 9 9 9   +   t m p i d + + ) ;  
         m y f t p C l i e n t . S e t F L ( l o c a l n a m e ) ;  
         m y f t p C l i e n t . S e t u f i l e s i z e ( m s i z e ) ;  
  
         U U T O O L B A R : : U s e r P r x   _ u s e r   =   M y I C E : : R ( ) ;  
         i f ( _ u s e r = = 0 ) {  
             r e t u r n   f a l s e ;  
         }  
         s t a t i c   s t r i n g   f t p _ i p ;  
         s t a t i c   s h o r t   f t p _ p o r t = 0 ;  
         s t r i n g   e m a i l ( _ 6 b e e d _ s h a r e : : u _ g e t E m a i l ( ) ) ;  
         s t r i n g   f t p _ p a s s w o r d ( _ 6 b e e d _ s h a r e : : u _ g e t P a s s w o r d ( ) ) ;  
         i f ( f t p _ p o r t = = 0 ) {      
             i f ( ! _ u s e r - > G e t F T P I n f o F o r U p l o a d ( e m a i l , f t p _ i p , f t p _ p o r t ) ) {  
                 r e t u r n   0 ;  
             }  
         }  
  
         _ 6 b e e s _ s o c k e t : : C L o g o n I n f o   l o g o n I n f o ( w s t r i n g m a k e r ( ) < < f t p _ i p ,  
             f t p _ p o r t ,  
             w s t r i n g m a k e r ( ) < < e m a i l ,  
             w s t r i n g m a k e r ( ) < < f t p _ p a s s w o r d ) ;  
         i f ( m y f t p C l i e n t . L o g i n ( l o g o n I n f o ) ) {  
             b o o l   u l f i n i s h e d   =   m y f t p C l i e n t . U p l o a d F i l e ( l o c a l n a m e , w s t r i n g m a k e r ( ) < < L " / " < < r e m o t e n a m e ) ;  
             i f   ( u l f i n i s h e d ) {  
                 l o n g   O L B K i d = _ w t o l ( i d ) ;  
                 s t r i n g   s i d ( _ 6 b e e d _ s h a r e : : u _ g e t S I D ( ) ) ;  
                 s t r i n g   r n a m e   =   s t r i n g m a k e r ( )   < <   r e m o t e n a m e ;  
                 b o o l   r e = f a l s e ;  
                 i f   ( m o d e = = U P L O A D _ T H U M B _ M O D E ) {  
                     i f ( _ u s e r - > P o s t U p l o a d T h u m b ( r n a m e , O L B K i d , s i d ) ) {  
                         : : D e l e t e F i l e W ( l o c a l n a m e ) ;  
                     }  
                 } e l s e   i f   ( m o d e = = U P L O A D _ I C O N _ M O D E ) {  
                     i f ( _ u s e r - > P o s t U p l o a d I c o n ( r n a m e , O L B K i d , s i d ) ) {  
                         : : D e l e t e F i l e W ( l o c a l n a m e ) ;  
                     }  
                 }  
             }  
             r e t u r n   t r u e ;  
         } e l s e {  
             r e t u r n   f a l s e ;  
         }  
     }  
  
     v o i d   N o t i f y 6 B e e T B ( i n t   m s g , H W N D   h ) {  
         s w i t c h ( _ 6 b e e d _ s h a r e : : u _ g e t i e v e r s i o n ( ) ) {  
             c a s e   8 :  
                 _ 6 b e e s _ w i n d o w : : I E 8 : : N o t i f y 6 B e e T B ( m s g , h ) ;  
                 b r e a k ;  
             c a s e   7 :  
                 _ 6 b e e s _ w i n d o w : : I E 7 : : N o t i f y 6 B e e T B ( m s g , h ) ;  
                 b r e a k ;  
             c a s e   6 :  
                 _ 6 b e e s _ w i n d o w : : I E 6 : : N o t i f y 6 B e e T B ( m s g , h ) ;  
                 b r e a k ;  
         }  
     }  
  
     H W N D   G e t R e f l e c t i o n W n d ( ) {  
         s w i t c h ( _ 6 b e e d _ s h a r e : : u _ g e t i e v e r s i o n ( ) ) {  
         c a s e   8 :  
             r e t u r n   _ 6 b e e s _ w i n d o w : : I E 8 : : G e t R e f l e c t i o n W n d ( ) ;  
         c a s e   7 :  
             r e t u r n   _ 6 b e e s _ w i n d o w : : I E 7 : : G e t R e f l e c t i o n W n d ( ) ;  
         c a s e   6 :  
             r e t u r n   _ 6 b e e s _ w i n d o w : : I E 6 : : G e t R e f l e c t i o n W n d ( ) ;  
         }  
         r e t u r n   N U L L ;  
     }  
  
     U I N T   W I N A P I   C h e c k V e r s i o n _ i n t e r n a l ( P V O I D   p ) {  
         t r y {  
             s t r i n g   u r l = ( c o n s t   c h a r * ) p ;  
             i n t   c l e n = 0 ;  
             c h a r   * d a t a   =   _ 6 b e e s _ n e t : : G e t D a t a B y H T T P ( u r l . c _ s t r ( ) , c l e n ) ;  
             i f   ( d a t a ! = N U L L ) {  
                 s t r i n g   a u r l   =   s t r i n g m a k e r ( )     < <   d a t a ;  
                 d e l e t e   [ ]   d a t a ;  
                 i f ( a u r l ! = " 0 "   & &   _ 6 b e e s _ s t r : : s t a r t w i t h < s t r i n g > ( a u r l , " h t t p : / / " ) ) {  
 # i f d e f   _ _ 6 B E E U P L O A D E R _ _  
                     s t r i n g   i n f o   =   s t r i n g m a k e r ( )   < <   " �`}Y, " < <   P R O D U C T N A M E   < < " 	g�f�e�vHr,g�S^�N, �s(W�SN}�GS�~�eHr,gT? " ;  
                     i f ( : : M e s s a g e B o x A ( N U L L , i n f o . c _ s t r ( ) , " �cKm0R�eHr,g" , M B _ Y E S N O ) = = I D Y E S ) {  
                         S h e l l E x e c u t e A ( N U L L , " o p e n " , " i e x p l o r e . e x e " , a u r l . c _ s t r ( ) , " " , S W _ S H O W ) ;  
                     }  
 # e l s e  
 # i f d e f   _ _ C H I N E S E _ _  
                     s t r i n g   i n f o   =   s t r i n g m a k e r ( )   < <   " �`}Y, " < <   P R O D U C T N A M E   < < " 	g�f�e�vHr,g�S^�N, �s(W�SN}�GS�~�eHr,gT? " ;  
                     i f ( : : M e s s a g e B o x A ( N U L L , i n f o . c _ s t r ( ) , " �cKm0R�eHr,g" , M B _ Y E S N O | M B _ I C O N I N F O R M A T I O N ) = = I D Y E S ) {  
                         S h e l l E x e c u t e A ( N U L L , " o p e n " , " i e x p l o r e . e x e " , a u r l . c _ s t r ( ) , " " , S W _ S H O W ) ;  
                     }  
 # e l s e  
                     s t r i n g   i n f o   =   s t r i n g m a k e r ( )   < <   P R O D U C T N A M E   < < "   h a s   n e w   v e r s i o n   a v a i l a b l e .   W o u l d   y o u   l i k e   t o   g o   t o   d o w n l o a d   t h e   l a t e s t   v e r s i o n ? " ;  
                     i f ( : : M e s s a g e B o x A ( N U L L , i n f o . c _ s t r ( ) , " N e w   V e r s i o n   A v a i l a b l e " , M B _ Y E S N O | M B _ I C O N I N F O R M A T I O N ) = = I D Y E S ) {  
                         S h e l l E x e c u t e A ( N U L L , " o p e n " , " i e x p l o r e . e x e " , a u r l . c _ s t r ( ) , " " , S W _ S H O W ) ;  
                     }  
 # e n d i f  
 # e n d i f  
                      
                 }  
             }  
         } c a t c h   ( . . . ) { }  
         r e t u r n   0 ;  
     }  
  
     s t r i n g   c h e c k u r l _ u r l ;  
     v o i d   C h e c k V e r s i o n ( c o n s t   w c h a r _ t *   f i l e p a t h ) {  
         w s t r i n g   f i l e n a m e =   P a t h F i n d F i l e N a m e W ( f i l e p a t h ) ;  
         L O W E R W S T R ( f i l e n a m e ) ;  
         v e r s i o n b l k   v e r b l k = G e t V e r s i o n B l k ( f i l e p a t h ) ;  
         c h e c k u r l _ u r l   =   _ 6 b e e s _ s t r : : s t r i n g m a k e r ( ) < <   _ 6 b e e s _ c o n s t : : k V e r s i o n C h k U R L   < <    
             " ? f i l e n a m e = "   < <   f i l e n a m e   < <   " & v e r s i o n = "   < <   v e r b l k . w M a j o r   < <  
             " . "   < <   v e r b l k . w M i n o r   < <  
             " . "   < <   v e r b l k . w R e l e a s e   < <  
             " . "   < <   v e r b l k . w B u i l d ;  
         : : _ b e g i n t h r e a d e x ( N U L L , 0 , C h e c k V e r s i o n _ i n t e r n a l , ( L P V O I D ) c h e c k u r l _ u r l . c _ s t r ( ) , 0 , 0 ) ;  
     }  
  
 } ;  
  
  
  
 n a m e s p a c e   _ 6 b e e d _ s h a r e {  
     u s i n g   n a m e s p a c e   _ 6 b e e s _ c o n s t ;  
     u s i n g   n a m e s p a c e   _ 6 b e e d _ u t i l ;  
  
     / /   h e l p e r   f u n c  
     C p p S Q L i t e 3 D B &   G e t D B R e f ( ) {  
         u s i n g   _ 6 b e e s _ u t i l : : u l t r a p a t h ;  
         s t r i n g   d b p a t h   =   u l t r a p a t h : : R ( ) . g e t A ( u l t r a p a t h : : m a i n d b ) ;  
         r e t u r n   C p p S Q L i t e 3 D B : : i n s t a n c e ( d b p a t h . c _ s t r ( ) ) ;  
     }  
  
     g c o n f : : g c o n f ( i n t ) : i e v e r s i o n ( G e t V e r s i o n B l k ( L " C : \ \ P r o g r a m   F i l e s \ \ I n t e r n e t   E x p l o r e r \ \ i e x p l o r e . e x e " ) . w M a j o r ) {  
         G e t 6 B e e L o c a t i o n ( u h o m e _ , M A X _ P A T H ) ;  
         C p p S Q L i t e 3 D B &   D B R e f   =   C p p S Q L i t e 3 D B : : i n s t a n c e ( G e t 6 B E E P a t h A ( L " m a i n . d b " ) ) ;  
         i f   ( F i n d W i n d o w E x ( N U L L , N U L L , N U L L , k U p l o a d e r _ W n d T i t l e )   = =   N U L L ) {  
             D B R e f . e x e c D M L ( " u p d a t e   U U C o n f i g   s e t   S I D = ' ' " ) ;  
         }  
         C p p S Q L i t e 3 T a b l e   t b l   =   D B R e f . g e t T a b l e ( " s e l e c t   *   f r o m   U U C o n f i g   l i m i t   1 " ) ;  
         s t r c p y _ s ( v e r s i o n , 1 1 , t b l . f i e l d V a l u e ( U _ 6 B E E V E R ) ) ;  
         s t r c p y _ s ( e m a i l , 5 0 , t b l . f i e l d V a l u e ( U _ N A M E ) ) ;  
         s t r c p y _ s ( p a s s w o r d , 3 2 + 1 , t b l . f i e l d V a l u e ( U _ P A S S ) ) ;  
         s t r c p y _ s ( s i d , 3 6 + 1 , t b l . f i e l d V a l u e ( U _ S I D ) ) ; / / l e n ( ' F 6 3 F 3 2 E D - 4 E 7 0 - 4 9 8 1 - 9 F 0 2 - 3 2 C F 5 2 9 4 7 8 C B ' )   =   3 6  
  
         l o g g e d i n     =   I S T R U E ( t b l . f i e l d V a l u e ( U _ L O G G E D I N ) )   & &   s t r l e n ( s i d ) = = 3 6 ;  
         a u t o l o g i n   =   I S T R U E ( t b l . f i e l d V a l u e ( U _ A U T O L O G I N ) ) ;  
  
         s e a r c h b o x l e n = a t o i ( t b l . f i e l d V a l u e ( U _ S E B O X L E N ) ) ;  
  
         i e _ h i d e b u i l t i n S E B o x = I S T R U E ( t b l . f i e l d V a l u e ( U _ I E _ B U I L T I N S E B O X ) ) ;  
         i e _ o n e w i n d o w = I S T R U E ( t b l . f i e l d V a l u e ( U _ I E _ O N E W I N D O W ) ) ;  
         l o a d f o r e x p l o r e r = I S T R U E ( t b l . f i e l d V a l u e ( U _ L O A D F O R E X P L O R E R ) ) ;  
     }  
  
     / /   1 .   U s e r   D a t a  
     / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /  
     v o i d   u p d a t e _ G e n e r a l S e t t i n g ( ) {  
         C p p S Q L i t e 3 T a b l e   t b l   =   G e t D B R e f ( ) . g e t T a b l e ( " s e l e c t   *   f r o m   U U C o n f i g   l i m i t   1 " ) ;  
         g l o b a l c o n f i g . i e _ h i d e b u i l t i n S E B o x = I S T R U E ( t b l . f i e l d V a l u e ( U _ I E _ B U I L T I N S E B O X ) ) ;  
         g l o b a l c o n f i g . i e _ o n e w i n d o w     = I S T R U E ( t b l . f i e l d V a l u e ( U _ I E _ O N E W I N D O W ) ) ;  
         g l o b a l c o n f i g . l o a d f o r e x p l o r e r   =   I S T R U E ( t b l . f i e l d V a l u e ( U _ L O A D F O R E X P L O R E R ) ) ;  
     }  
     b o o l   u _ g e t L o g g e d i n ( ) {  
         r e t u r n   g l o b a l c o n f i g . l o g g e d i n ;  
     }  
     v o i d   u _ s e t L o g g e d i n ( b o o l   b ) {  
         i f   ( g l o b a l c o n f i g . l o g g e d i n ! = b ) {  
             g l o b a l c o n f i g . l o g g e d i n = b ;  
             A T L : : C S t r i n g A   c m d ;  
             c m d . F o r m a t ( " u p d a t e   U U C o n f i g   s e t   l o g g e d i n = ' % s ' " , b ? " t r u e " : " f a l s e " ) ;  
             U U D E B U G ( ( L O G _ E R R O R , c m d . G e t S t r i n g ( ) ) ) ;  
             G e t D B R e f ( ) . e x e c D M L ( c m d ) ;  
         }  
     }  
     c o n s t   c h a r *   u _ g e t S I D ( ) {  
         r e t u r n   g l o b a l c o n f i g . s i d ;  
     }  
     v o i d   u _ s e t S I D ( c o n s t   c h a r *   s i d ) {  
         i f   ( s t r c m p ( g l o b a l c o n f i g . s i d , s i d ) ! = 0 ) {  
             s t r c p y _ s ( g l o b a l c o n f i g . s i d , 3 6 + 1 , s i d ) ;  
             A T L : : C S t r i n g A   c m d ;  
             c m d . F o r m a t ( " u p d a t e   U U C o n f i g   s e t   s i d = ' % s ' " , s i d ) ;  
             U U D E B U G ( ( L O G _ E R R O R , c m d . G e t S t r i n g ( ) ) ) ;  
             G e t D B R e f ( ) . e x e c D M L ( c m d ) ;  
         }  
     }  
     c o n s t   c h a r *   g e t _ v e r s i o n ( ) {  
         r e t u r n   g l o b a l c o n f i g . v e r s i o n ;  
     }  
     c o n s t   w c h a r _ t *   g e t _ u h o m e ( ) {  
         r e t u r n   g l o b a l c o n f i g . u h o m e _ ;  
     }  
     b o o l   u _ g e t A u t o l o g i n ( ) {  
         r e t u r n   g l o b a l c o n f i g . a u t o l o g i n ;  
     }  
     v o i d   u _ s e t A u t o l o g i n ( b o o l   b ) {  
         i f   ( g l o b a l c o n f i g . a u t o l o g i n ! = b ) {  
             g l o b a l c o n f i g . a u t o l o g i n = b ;  
             A T L : : C S t r i n g A   c m d ;  
             c m d . F o r m a t ( " u p d a t e   U U C o n f i g   s e t   a u t o l o g i n = ' % s ' " , b ? " t r u e " : " f a l s e " ) ;  
             G e t D B R e f ( ) . e x e c D M L ( c m d ) ;  
         }  
     }  
     c o n s t   c h a r *   u _ g e t P a s s w o r d ( ) {  
         r e t u r n   g l o b a l c o n f i g . p a s s w o r d ;  
     }  
     v o i d   u _ s e t P a s s w o r d ( c o n s t   c h a r *   p ) {  
         i f   ( s t r c m p ( g l o b a l c o n f i g . p a s s w o r d , p ) ! = 0 ) {  
             s t r c p y _ s ( g l o b a l c o n f i g . p a s s w o r d , 3 2 + 1 , p ) ;  
             A T L : : C S t r i n g A   c m d ;  
             c m d . F o r m a t ( " u p d a t e   U U C o n f i g   s e t   p a s s w o r d = ' % s ' " , p ) ;  
             G e t D B R e f ( ) . e x e c D M L ( c m d ) ;  
         }  
     }  
     c o n s t   c h a r *   u _ g e t E m a i l ( ) {  
         r e t u r n   g l o b a l c o n f i g . e m a i l ;  
     }  
     v o i d   u _ s e t E m a i l ( c o n s t   c h a r *   e m a i l ) {  
         i f   ( s t r c m p ( g l o b a l c o n f i g . e m a i l , e m a i l ) ! = 0 ) {  
             s t r c p y _ s ( g l o b a l c o n f i g . e m a i l , 5 0 , e m a i l ) ;  
             A T L : : C S t r i n g A   c m d ;  
             c m d . F o r m a t ( " u p d a t e   U U C o n f i g   s e t   u s e r n a m e = ' % s ' " , e m a i l ) ;  
             G e t D B R e f ( ) . e x e c D M L ( c m d ) ;  
         }  
     }  
  
     / / 2 .   O t h e r   g l o b a l   s e t t i n g  
     / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /  
     i n t   u _ g e t S e a r c h b o x l e n ( ) {  
         r e t u r n   g l o b a l c o n f i g . s e a r c h b o x l e n ;  
     }  
     v o i d   u _ s e t S e a r c h b o x l e n ( i n t   l e n ) {  
         g l o b a l c o n f i g . s e a r c h b o x l e n = l e n ;  
         A T L : : C S t r i n g A   c m d ;  
         c m d . F o r m a t ( " u p d a t e   U U C o n f i g   s e t   s e a r c h b o x l e n g t h = % d " , l e n ) ;  
         G e t D B R e f ( ) . e x e c D M L ( c m d ) ;  
     }  
     b o o l   u _ g e t i e b u i l t i n S E B o x ( ) {  
         r e t u r n   g l o b a l c o n f i g . i e _ h i d e b u i l t i n S E B o x ;  
     }  
     b o o l   u _ s e t i e b u i l t i n S E B o x ( b o o l   b , b o o l   i s u p d a t e d b ) {  
         / /   b :   t r u e   -   h i d e , f a l s e   -   s h o w  
         b o o l   s u c c e s s f u l = f a l s e ;  
         C R e g K e y   c r k ;  
         l o n g   r k 2 = c r k . O p e n ( H K E Y _ L O C A L _ M A C H I N E ,  
             _ T ( " S o f t w a r e \ \ P o l i c i e s \ \ M i c r o s o f t \ \ I n t e r n e t   E x p l o r e r \ \ I n f o d e l i v e r y \ \ R e s t r i c t i o n s " ) ,  
             K E Y _ R E A D | K E Y _ W R I T E ) ;  
         i f   ( r k 2 = = E R R O R _ S U C C E S S ) {  
             r k 2 = c r k . S e t D W O R D V a l u e ( _ T ( " N o S e a r c h B o x " ) , b ? 1 : 0 ) ;  
             s u c c e s s f u l   =   ( r k 2 = = E R R O R _ S U C C E S S ) ;  
         } e l s e {  
             D W O R D   v = R E G _ C R E A T E D _ N E W _ K E Y ;  
             r k 2 = c r k . C r e a t e ( H K E Y _ L O C A L _ M A C H I N E ,  
                 _ T ( " S o f t w a r e \ \ P o l i c i e s \ \ M i c r o s o f t \ \ I n t e r n e t   E x p l o r e r \ \ I n f o d e l i v e r y \ \ R e s t r i c t i o n s " ) ,  
                 N U L L ,   R E G _ O P T I O N _ N O N _ V O L A T I L E , K E Y _ W R I T E | K E Y _ R E A D , N U L L , & v ) ;  
             i f   ( r k 2 = = E R R O R _ S U C C E S S ) {  
                 r k 2 = c r k . S e t D W O R D V a l u e ( _ T ( " N o S e a r c h B o x " ) , b ? 1 : 0 ) ;  
                 s u c c e s s f u l   =   ( r k 2 = = E R R O R _ S U C C E S S ) ;  
             }  
         }  
         i f   ( s u c c e s s f u l ) {  
             g l o b a l c o n f i g . i e _ h i d e b u i l t i n S E B o x = b ;  
             i f   ( i s u p d a t e d b ) {  
                 A T L : : C S t r i n g A   c m d ;  
                 c m d . F o r m a t ( " u p d a t e   U U C o n f i g   s e t   i e _ h i d e b u i l t i n S E B o x = ' % s ' " , b ? " t r u e " : " f a l s e " ) ;  
                 G e t D B R e f ( ) . e x e c D M L ( c m d ) ;  
             }  
         }  
         r e t u r n   s u c c e s s f u l ;  
     }  
     b o o l   u _ g e t i e o n e w i n d o w ( ) {  
         r e t u r n   g l o b a l c o n f i g . i e _ o n e w i n d o w ;  
     }  
     b o o l   u _ s e t i e o n e w i n d o w ( b o o l   b ) {  
         g l o b a l c o n f i g . i e _ o n e w i n d o w = b ;  
         A T L : : C S t r i n g A   c m d ;  
         c m d . F o r m a t ( " u p d a t e   U U C o n f i g   s e t   i e _ o n e w i n d o w = ' % s ' " , b ? " t r u e " : " f a l s e " ) ;  
         G e t D B R e f ( ) . e x e c D M L ( c m d ) ;  
         r e t u r n   t r u e ;  
     }  
     b o o l   u _ g e t l o a d f o r e x p l o r e r ( ) {  
         r e t u r n   g l o b a l c o n f i g . l o a d f o r e x p l o r e r ;  
     }  
     b o o l   u _ s e t l o a d f o r e x p l o r e r ( b o o l   b ) {  
         g l o b a l c o n f i g . l o a d f o r e x p l o r e r = b ;  
         A T L : : C S t r i n g A   c m d ;  
         c m d . F o r m a t ( " u p d a t e   U U C o n f i g   s e t   l o a d f o r e x p l o r e r = ' % s ' " , b ? " t r u e " : " f a l s e " ) ;  
         G e t D B R e f ( ) . e x e c D M L ( c m d ) ;  
         r e t u r n   t r u e ;  
     }  
     s h o r t   u _ g e t i e v e r s i o n ( ) {  
 	     r e t u r n   g l o b a l c o n f i g . i e v e r s i o n ;  
     }  
     v o i d   u _ s e t i e v e r s i o n ( s h o r t   v ) {  
         g l o b a l c o n f i g . i e v e r s i o n   =   v ;  
     }  
 } ; 