# Codes with locality from cyclic extensions of Deligne-Lusztig curves

Gretchen L. Matthews<sup>1</sup> and Fernando Piñero2

Department of Mathematics, Virginia Tech, Blacksburg, VA 24061 gmatthews@vt.edu?

Department of Mathematics, University of Puerto Rico at Ponce, Ponce, PR fernando.pinero1@upr.edu

Abstract. Recently, Skabelund defined new maximal curves which are cyclic extensions of the Suzuki and Ree curves. Previously, the now wellknown GK curves were found as cyclic extensions of the Hermitian curve. In this paper, we consider locally recoverable codes constructed from these new curves, complementing that done for the GK curve. Locally recoverable codes allow for the recovery of a single symbol by accessing only a few others which form what is known as a recovery set. If every symbol has at least two disjoint recovery sets, the code is said to have availability. Three constructions are described, as each best fits a particular situation. The first employs the original construction of locally recoverable codes from curves by Tamo and Barg. The second yields codes with availability by appealing to the use of fiber products as described by Haymaker, Malmskog, and Matthews, while the third accomplishes availability by taking products of codes themselves. We see that cyclic extensions of the Deligne-Lusztig curves provide codes with smaller locality than those typically found in the literature.

## 1 Introduction

Maximal curves have played a role in a number of applications in coding theory. For instance, they allow for the construction of long algebraic geometry codes and yield explicit families of codes with parameters exceeding the Gilbert-Varshamov bound [23]. The Deligne-Lusztig curves, which include the Hermitian, Suzuki, and Ree curves, have proven particularly useful. In particular, Hermitian codes are perhaps the best understood algebraic geometry codes other than Reed-Solomon codes. The Suzuki and Ree curves share several important properties with the Hermitian family in that they are optimal with respect to the Hasse-Weil bound and have known automorphism groups; thus, codes from these curves have interesting properties as well.

More recently, maximal curves have been employed in the construction of codes with locality. In some applications, it is desirable to recover a single (or small number of) codeword symbol(s) by accessing only a few, say $r ,$ particular symbols of the received word. This leads to the notion of locally recoverable codes, or LRCs. Tamo and Barg [21] introduced a construction for codes with locality that is similar to that of algebraic geometry codes. This motivated much work on locally recoverable codes, including [1], [2], [7], [11], [13]. In [9], we employ maximal curves to construct LRCs with availability $t \geq 2 .$ , meaning each coordinate $j$ has t disjoint recovery sets. Codes with availability make information more available to more users, since recovery of an erasure is not entirely dependent on a single set of coordinates (which may itself contain erasures).

<small><span class="docvortex-page-footnote" data-block-type="page_footnote" style="color:#6b7280">? Partially supported by NSF DMS-1855136.</span></small>

In this paper, we define codes with locality from new maximal curves constructed by Skabelund [18] using cyclic covers of the Suzuki and Ree curves. The Suzuki curve $S _ { q }$ over $\mathbb { F } _ { q }$ gets its name from its automorphism group which is the Suzuki group $S z ( q )$ of order $q ^ { 2 } ( q ^ { 2 } + 1 ) ( q - 1 )$ . In [8], Hansen and Stichtenoth considered this curve and applications to algebraic geometry codes leading to other works such as [12], [15]. Recently, Eid, Hammond, Ksir, and Peachey [4] constructed an algebraic geometry (AG) code over $\mathbb { F } _ { q ^ { 4 } }$ whose automorphism group is $S z ( q )$ . Skabelund considers a cyclic extension of $S _ { q }$ and proves it is maximal over $\mathbb { F } _ { q }$ and $\mathbb { F } _ { q ^ { 4 } }$ . Similarly, the Ree curve $R _ { q }$ over $\mathbb { F } _ { q }$ has a Ree group as its automorphism group. Both curve constructions are similar to that of the Giulietti-Korchmáros, or GK, curve, which has already proven useful in constructing codes with locality. These cyclic extensions of the Suzuki and Ree curves have also been utilized for AG codes and for quantum codes from them [16] and their automorphism groups have been determined by Giulietti, Montanucci, Quoos, and Zini [6].

This paper is organized as follows. In Section 2, we obtain codes with locality from the cyclic extension $\tilde { S } _ { q }$ of the Suzuki curve $S _ { q }$ and the cyclic extension $\tilde { R } _ { q }$ of the Ree curve $R _ { q } ,$ . The locality is much smaller relative to the alphabet size and code length than comparable constructions. In Section 3, we construct codes with availability from $\tilde { S } _ { q }$ and $\tilde { R } _ { q } ,$ . Our constructions build on tools found in [21] and [9], and some useful background may be found there. Because explicit code descriptions remain out of reach for these standard constructions when employing $R _ { q }$ or $\tilde { R } _ { q }$ (as they depend on explicit bases for Riemann-Roch spaces which remain elusive), we provide an alternate construction for such settings. We also consider constructions from products of codes. In Section 4, we consider examples of the above constructions and make some comparisons between them.

## 2 Locally recoverable codes

Locally recoverable codes, or LRCs for short, can recover a single (or small number of) codeword symbol(s) by accessing a small number, say $r ,$ of particular symbols of the received word. In principle, the locality r should be small so as to limit network traffic though this can adversely impact other code parameters. While an $[ n , k , d ]$ code $C ,$ meaning a code of length $n ,$ dimension $k ,$ and minimum distance $d ,$ can recover any $d { - } 1$ erasures or correct any $\left[ { \frac { d - 1 } { 2 } } \right]$ errors, this assumes access to all other symbols of the entire received word. More precisely, the code

C of length n over the alphabet F (typically taken to be a finite field) is locally recoverable with locality r if and only if for all $j \in [ n ] : = \{ 1 , \ldots , n \}$ there exists

$$
A _ {j} \subseteq [ n ] \setminus \{j \} \text {with} | A _ {j} | = r
$$

and

$$
c _ {j} = \phi_ {j} (c | _ {A _ {j}})
$$

for some function

$$
\phi_ {j}: A _ {j} \to \mathbb {F}
$$

for all $c \in C$ . The set $A _ { j }$ is called a recovery set for the j-th coordinate. In this section, we see how cyclic extensions naturally lead to LRCs.

## 2.1 LRCs from cyclic extensions of Suzuki curves

The Suzuki curve $S _ { q }$ may be described by the equation

$$
S _ {q}: y ^ {q} + y = x ^ {q _ {0}} (x ^ {q} + x)
$$

where $q _ { 0 } = 2 ^ { s } ,   q = 2 q _ { 0 } ^ { 2 }$ , and $s \in \mathbb { N }$ . It is an optimal curve over $\mathbb { F } _ { q } ,$ having $q ^ { 2 } + 1$ $\mathbb { F } _ { q ^ { * } }$ -rational points. Indeed, if $a , b   \in   \mathbb { F } _ { q } ,   a ^ { q }   =   a$ and $b ^ { q }   =   b ;$ since char $\mathbb { F } _ { q }   =   2 ,$ $\vec { b ^ { q } } + \vec { b }   =   0   =   a ^ { q _ { 0 } } \left( a ^ { q } + a \right)$ . In addition, there is a unique point at infinity $P _ { \infty }$ corresponding to $x = z = 0$ and $y = 1$ . The genus of $S_{q}\  is \ q_{0}\left ( q-1 \right )$ [8, Lemma 1.9]. It is maximal over $\mathbb { F } _ { q ^ { 4 } }$ , having $q ^ { 4 } + 1 + 2 q _ { 0 } q ^ { 2 } \dot { ( q - 1 ) } \; \mathbb { F } _ { q ^ { 4 } }$ -rational points $[ 4$ Equation (7)]. Define

$$
\tilde {S} _ {q}: \left\{ \begin{array}{l} y ^ {q} + y = x ^ {q _ {0}} (x ^ {q} + x) \\ t ^ {m} = x ^ {q} + x. \end{array} \right.
$$

where $m = q - 2 q _ { 0 } + 1$ . The curve $\tilde { S } _ { q }$ has a unique point at infinity, and affine points will be denoted $P _ { a b c } : = ( a : b : c : 1 )$ to mean the unique zero of $x - a .$ $y - b ,$ and $t - c ,$ just as those of $S _ { q }$ will be denoted by $P _ { a b }$ . The genus of $\tilde { S } _ { q }$ is $\scriptstyle { \frac { q ^ { 3 } - 2 q ^ { 2 } + q } { 2 } } [ 1 8 ]$ . According to [18], the number of $\mathbb { F } _ { q ^ { 4 } }$ -rational points on $\tilde { S } _ { q }$ that are not $\mathbb { F } _ { q ^ { \ast } }$ -rational is

$$
q ^ {5} - q ^ {4} + q ^ {3} - q ^ {2};
$$

see also [18, Section 3] for a discussion of the points on this curve. Define

$$
\begin{array}{c} g: \tilde {S} _ {q} \to S _ {q} \\ P _ {a b c} \mapsto P _ {a b} \end{array}
$$

Let

$$
S := S _ {q} \left(\mathbb {F} _ {q ^ {4}}\right) \setminus S _ {q} \left(\mathbb {F} _ {q}\right).\tag{1}
$$

Then $| S | = q ^ { 4 }   +   2 q _ { 0 } q ^ { 2 } ( q   -   1 )   -   q ^ { 2 }$ [4, Equations (4)-(7)]. Set $\textstyle D : = \sum _ { P \in { \mathcal { D } } } P$ where

$$
\mathcal {D} := g ^ {- 1} (S) = \left\{P _ {a b c} \in \tilde {S} _ {q} \left(\mathbb {F} _ {q ^ {4}}\right): c \neq 0 \right\}.\tag{2}
$$

For each $P _ { a b } \in S ,   g ^ { - 1 } \left( P _ { a b } \right) = \{ P _ { a b c } : c ^ { m } = a ^ { q } + a \} ,$ so

$$
| g ^ {- 1} (P _ {a b}) | = q - 2 q _ {0} + 1.\tag{3}
$$

Recall that given a divisor $G$ on a curve $X$ over a field $\mathbb { F } ,$ the space of functions determined by $G ,$ sometimes called the Riemann-Roch space of $G ,$ is

$$
\mathcal {L} (G) := \{f \in \mathbb {F} (X): (f) \geq - G \} \cup \{0 \},
$$

where $\mathbb { F } ( X )$ denotes the set of rational functions on $X .$ , and $( f )$ denotes the divisor of the function $f ;$ to say that $\textstyle ( f ) \; = \; \sum _ { Q \in \mathcal { Z } } a _ { Q } Q   -   \sum _ { P \in \mathcal { P } } b _ { P } P$ with $a _ { Q } , b _ { P }   \in   \mathbb { Z } ^ { + }$ means $f$ has a zero of order $a _ { Q }$ at $Q$ and a pole of order $b _ { P }$ at $P .$ We use the standard notation $( f ) _ { 0 } : = \textstyle \sum _ { Q \in \mathcal { Z } } a _ { Q } Q$ to denote the zero divisor of $f$ and $\textstyle ( f ) _ { \infty } : = \sum _ { P \in \mathcal { P } } b _ { P } P$ to denote the pole divisor of $f .$ Let $\alpha \in \mathbb { Z } ^ { + }$ , and consider the divisor

$$
G := \alpha \left(P _ {\infty} + \sum_ {a, b \in \mathbb {F} _ {q}} P _ {a b}\right)
$$

on $S _ { q }$ . It is worth noting that $\mathcal { L } \left( \alpha \left( P _ { \infty } + \textstyle \sum _ { a , b \in \mathbb { F } _ { q } } P _ { a b } \right) \right) { \cong } \mathcal { L } \left( \alpha \left( q ^ { 2 } + 1 \right) P _ { \infty } \right)$ [4]. According to [4, Theorem $1 ] ,$ a basis for ${ \mathcal { L } } ( G )$ is given by

$$
\mathcal {B} := \left\{ \begin{array}{c c} & a q + b (q + q _ {0}) + c (q + 2 q _ {0}) \\ \frac {x ^ {a} y ^ {b} u ^ {c} v ^ {d}}{(x ^ {q} + x) ^ {e}}: & + d (q + 2 q _ {0} + 1) \leq \alpha + e q ^ {2} \\ & a \in \{0, \dots , q - 1 \}, b \in \{0, 1 \}, \\ & c, d \in \{0, \dots , q _ {0} - 1 \}, e \in \{0, \dots , \alpha \} \end{array} \right\} \subseteq \mathbb {F} _ {q ^ {4}} \left(S _ {q}\right)
$$

where

$$
u = x ^ {2 q _ {0} + 1} - y ^ {2 q _ {0}}
$$

and

$$
v = x y ^ {2 q _ {0}} - u ^ {2 q _ {0}}.
$$

Set

$$
V := \left\langle f t ^ {i}: i = 0, \dots , m - 2; f \in \mathcal {B} \right\rangle_ {\mathbb {F} _ {q ^ {4}}}.
$$

Now define

$$
\begin{array}{c} e v: V \to \mathbb {F} _ {q ^ {4}} ^ {(q - 2 q _ {0} + 1) \left(q ^ {4} + 2 q _ {0} q ^ {2} (q - 1) - q ^ {2}\right)} \\ f \mapsto \big (f \left(P _ {a b c}\right) \big) _ {P _ {a b c} \in \tilde {S} _ {q} \left(\mathbb {F} _ {q ^ {4}}\right) \setminus \tilde {S} _ {q} (\mathbb {F} _ {q})}, \end{array}
$$

and set $C ( D , G , g ) : = e v ( V )$ . Note that the evaluation map ev is well-defined, as $\mid \mathcal { D } \mid = ( q - 2 q _ { 0 } + 1 ) ( q ^ { 4 } + 2 q _ { 0 } q ^ { 2 } ( q - 1 ) - q ^ { 2 } )$ and $f \in V$ has no poles at points in D. One may notice that

$$
V \subseteq \mathcal {L} \left(\left(m \alpha + (m - 2) q ^ {2}\right) \tilde {P} _ {\infty} + m \alpha \sum_ {a, b \in \mathbb {F} _ {q}} P _ {a, b, 0}\right)\tag{4}
$$

where $\tilde { P } _ { \infty }$ denotes the unique point of $\tilde { S } _ { q }$ lying above $P _ { \infty }$ . Let

$$
G ^ {\prime} := \left(m \alpha + (m - 2) q ^ {2}\right) \tilde {P} _ {\infty} + m \alpha \sum_ {a, b \in \mathbb {F} _ {q}} P _ {a, b, 0}.
$$

We are now ready to state the result.

Theorem 1. Suppose $C ( D , G , g )$ is constructed as above where deg $G ^ { \prime }   <   | S |$ Then $C ( D , G , g )$ is an $[ n , k , d ]$ code over $\mathbb { F } _ { q ^ { 4 } }$ with locality $q - 2 q _ { 0 }$ ,

$$
n = (q - 2 q _ {0} + 1) \left(q ^ {4} + 2 q _ {0} q ^ {2} (q - 1) - q ^ {2}\right),
$$

$$
k = (q - 2 q _ {0}) (\alpha (q ^ {2} + 1) - q _ {0} (q - 1) + 1),
$$

and

$$
d \geq n - \left(m \alpha q ^ {2} + m \alpha + (m - 2) q ^ {2}\right).
$$

Proof. The map ev is injective, since deg $G ^ { \prime } < | S |$ guarantees the kernel of the evaluation map is {0}; this may be observed by noting that if $f \in \ker e v \setminus \{ 0 \}$ then f would have more zeros than poles. Hence, the dimension is given by di $\operatorname { m } _ { \mathbb { F } q ^ { 4 } } V$ which follows from the facts that $\left\{ t ^ { i } : i = 0 , 1 , \ldots , m - 1 \right\}$ is a basis of $\mathbb { F } _ { q ^ { 4 } } ( \tilde { S } _ { q } ) / \mathbb { F } _ { q ^ { 4 } } ( S _ { q } )$ ; B is a basis for $\mathbb { F } _ { q ^ { 4 } } ( S _ { q } ) / \mathbb { F } _ { q ^ { 4 } } ;$ and

$$
\mid \mathcal {B} \mid = \left(\alpha \left(q ^ {2} + 1\right) - q _ {0} (q - 1) + 1\right)
$$

according to $[ 4 ,$ Remark 1]. We claim that $R : = g ^ { - 1 } \left( P _ { a b } \right) \setminus \left\{ P _ { a b c } \right\}$ is a recovery set for the position corresponding to $P _ { a b c }$ . Suppose $f \in V$ . Then

$$
f (x, y, t) = \sum_ {i = 0} ^ {m - 2} \sum_ {j = 1} ^ {M} a _ {i j} f _ {j} ^ {*} t ^ {i}
$$

for some $a _ { i j } \in \mathbb { F } _ { q ^ { 4 } }$ and $f _ { j } ^ { * } \in \mathcal { B } ,$ where $M : = \mid { \mathcal { B } } \mid$ . Notice that $f ( a , b , T ) \in \mathbb { F } _ { q } \left[ T \right]$ and $\deg _ { T } f ( a , b , { \dot { T } } ) \leq { \dot { m - 2 } }$ . Hence, $f ( a , b , c )$ can be recovered using the $m - 1$ interpolation points: $P _ { a b c ^ { \prime } } \in R$ . As a result, $f \left( P _ { a b c } \right)$ may be recovered using only elements of R.

To determine a bound on the minimum distance $d ,$ we use that

$$
d \geq w t (e v (h)) \geq n - \deg (h) _ {0}
$$

where $h = f t ^ { m - 2 }$ and $f \in { \mathcal { B } } \subseteq { \mathcal { L } } ( G )$ . Then $\operatorname { d e g } ( h ) _ { 0 } \geq m \operatorname { d e g } ( G ) + ( m - 2 ) q ^ { 2 } \geq$ mα $\iota ( q ^ { 2 }   +   1 )   +   ( m   -   2 ) q ^ { 2 }$ as G is a divisor of degree $\alpha ( q ^ { 2 }   +   1 )$ on $S _ { q } ,   [ \tilde { S } _ { q } : S _ { q } ] = m$ and (t) is a divisor on $\tilde { S } _ { q }$ with zero divisor of degree $q ^ { 2 }$ . As a result

$$
d \geq n - \left(m \alpha (q ^ {2} + 1) + (m - 2) q ^ {2}\right).
$$

Alternatively, the bound on the minimum distance may be seen as a consequence of $d \geq n - \deg G ^ { \prime }$ using (4).

Example 1. Let $q = 8$ and $q _ { 0 } = 2$ , so $q ^ { 4 } = 4 0 9 6$ . Notice that the Suzuki curve

$$
S _ {8}: y ^ {8} + y = x ^ {2} (x ^ {8} + x)
$$

has 64 F<sub>8</sub>-rational points and 5888 $\mathbb { F } _ { 4 0 9 6 ^ { \circ } }$ rational points. Here, $| S | = 5 8 2 4$ and $n = 2 9 1 2 0$ . Then $C ( D , G , g )$ has locality 4. We can compare this with an LRC $C ^ { \prime }$ from the Hermitian curve $y ^ { 6 4 } + y   =   x ^ { 6 5 }$ over the same field, $\mathbb { F } _ { 4 0 9 6 }$ . Using a projection onto the x-coordinate gives a code of length 262144 with locality 63 whereas projection onto the $y -$ coordinate yields locality 64. Hence, the construction using ${ \tilde { S } } _ { 8 }$ has a smaller ratios of locality to code length and to alphabet size.

Remark 1. 1. Other bounds on the minimum distance of the codes in Theorem 1 may be given; see [22] for instance.

2. Alternatively, an LRC may be constructed using the projection

$$
\begin{array}{c} g: \tilde {S} _ {q} \to C _ {m} \\ P _ {a b c} \mapsto Q _ {a c} \end{array}
$$

where $C _ { m }$ denotes the curve given by $t ^ { m }   =   x ^ { q } + x$ and $Q _ { a c }$ denotes the common zero of $x   -   a$ and $t   -   c .$ . Let S be as in $( 1 ) , D$ as in $( 2 )$ , and $G ^ { \prime } : = \alpha Q _ { \infty }$ where $Q _ { \infty }$ is the point at infinity on $C _ { m }$ . Then a basis for $\mathcal { L } \left( \alpha Q _ { \infty } \right)$ is given by

$$
\mathcal {B} ^ {\prime} := \left\{t ^ {i} x ^ {j}: i \geq 0, j \in \{0, \dots , q - 1 \}, q i + m j \leq \alpha \right\};
$$

see, for instance, [10, Lemma 12.2(i)]. Use this to define

$$
V = \left\langle f y ^ {i}: i \in \{0, \dots , q - 2 \}, f \in \mathcal {B} ^ {\prime} \right\rangle .
$$

The code $C ( D , G ^ { \prime } , g )$ has locality $q - 1$ and dimension $\left( q - 1 \right) \left| \mathcal { B } \right|$

In Section 3, we will see how these two approaches can be combined to give LRCs with availability. Before doing so, we turn our attention to cyclic extensions of Ree curves.

## 2.2 LRCs from cyclic extensions of Ree curves

The Ree curve $R _ { q }$ may be described by the equation

$$
R _ {q}: \left\{ \begin{array}{l} y ^ {q} - y = x ^ {q _ {0}} \left(x ^ {q} - x\right) \\ z ^ {q} - z = x ^ {2 q _ {0}} \left(x ^ {q} - x\right) \end{array} \right.
$$

where $q _ { 0 } = 3 ^ { s } ,   q = 3 q _ { 0 } ^ { 2 }$ , and $s \in \mathbb { N }$ . It is optimal over $\mathbb { F } _ { q ^ { 6 } }$ . In addition, there is a unique point at infinity. The genus of $R _ { q }$ is $\textstyle { \frac { 3 } { 2 } } q _ { 0 } \left( q - 1 \right) \dot { \left( \right)} q + q _ { 0 } + 1$ [8]. Define

$$
\tilde {R} _ {q}: \left\{ \begin{array}{l} y ^ {q} - y = x ^ {q _ {0}} \left(x ^ {q} - x\right) \\ z ^ {q} - z = x ^ {2 q _ {0}} \left(x ^ {q} - x\right) \\ t ^ {m} = x ^ {q} - x \end{array} \right.
$$

where $m = q - 3 q _ { 0 } + 1$ . The curve $\tilde { R } _ { q }$ has a unique point at infinity, and affine points will be denoted $P _ { a b c d } : = ( a : \vec { b : c : d : 1 } )$ to mean the unique zero of $x - a ,$ $y - b,   z - c$ and $t - d ,$ just as those of $R _ { q }$ will be denoted by $P _ { a b c }$ . The genus of $\tilde { R } _ { q }$ is $\frac { q ^ { 4 } - 2 q ^ { 3 } + q } { 2 }$ . According to [16], the number of $\mathbb { F } _ { q ^ { 6 } }$ -rational points on $\tilde { R } _ { q }$ that are not $\mathbb { F } _ { q ^ { * } } ^ { - }$ -rational is

$$
q ^ {7} - q ^ {6} + q ^ {4} - q ^ {3};
$$

see also [18]. Define

$$
\begin{array}{c} g: \tilde {R} _ {q} \to R _ {q} \\ P _ {a b c d} \mapsto P _ {a b c} \end{array}
$$

and let

$$
S := R _ {q} \left(\mathbb {F} _ {q ^ {0}}\right) \setminus R _ {q} \left(\mathbb {F} _ {q}\right).\tag{5}
$$

Set $\textstyle D : = \sum _ { P \in { \mathcal { D } } } P$ where

$$
\mathcal {D} := g ^ {- 1} (S) = \left\{P _ {a b c d} \in \tilde {R} _ {q} \left(\mathbb {F} _ {q ^ {6}}\right): d \neq 0 \right\}.\tag{6}
$$

For each $P_{abc} \in S,   g^{-1} \left( P_{abc} \right) = \left\{ P_{abc} : d^m = a^q - a \right\}$ , so

$$
| g ^ {- 1} (P _ {a b c}) | = q - 3 q _ {0} + 1.\tag{7}
$$

Consider the divisor $G = \alpha P _ { \infty }$ on $R _ { q }$ with m deg $G + ( m - 2 ) \deg ( t ) _ { \infty } < | S |$ Set

$$
V := \left\langle f t ^ {i}: i = 0, \dots , m - 2; f \in \mathcal {L} (G) \right\rangle_ {\mathbb {F} _ {q ^ {6}}}.
$$

Now define

$$
\left| \begin{array}{c} e v: V \to \mathbb {F} _ {q ^ {6}} ^ {| \mathcal {D} |} \\ f \mapsto (f (P _ {a b c d})) _ {P _ {a b c d} \in \tilde {R} _ {q} (\mathbb {F} _ {q ^ {6}}) \setminus \tilde {R} _ {q} (\mathbb {F} _ {q})}, \end{array} \right.
$$

and set $C ( D , G , g ) : = e v ( V )$

Proposition 1. Suppose $C ( D , G , g )$ is constructed as above. Then $C ( D , G , g )$ is an $[ q ^ { 7 } - q ^ { 6 } + q ^ { 4 } - q ^ { 3 } , \dot { ( m - 1 ) } \ell ( G ) ]$ code over $\mathbb { F } _ { q ^ { 6 } }$ with locality $q - 3 q _ { 0 }$

Proof. This follows similarly to that of Theorem 1.

Remark 2. 1. Explicit bases for ${ \mathcal { L } } ( G )$ where G is a divisor on the Ree curve is a topic of current research for arbitrary q, even for the case where G is a multiple of the point at infinity. See [19] for recent work on related topics. The work [3] also highlights the challenges of this problem, which was originally stated in [17]; indeed, when $s = 1 ( \mathrm { s o } q = 2 7 )$ , the associated Weierstrass semigroup has more than 100 generators, compared with 2 in the Hermitian case and 4 for Suzuki. Hence, the dimension of the codes described in Proposition 1 cannot be specified more precisely the expression given above for arbitrary $q .$ However, for specific small values of $q ,$ a set of functions which generate ${ \mathcal { L } } ( G )$ may be found computationally. We include this result so that if the theory progresses and sheds more light on this value, LRCs are an immediate consequence. We also note that our interest in the Ree curve is partially motivated by the fact that it allows for results over fields of odd characteristic whose cardinalities are odd powers of primes (unlike the Suzuki curve, which is considered over a field of even characteristic, and the Hermitian curve which is considered over a field with square cardinality).

2. Also, as in Remark 1, the projection

$$
\begin{array}{c} g: \tilde {R} _ {q} \to C _ {m} \\ P _ {a b c d} \mapsto Q _ {a d}, \end{array}
$$

where $C _ { m }   :   t ^ { m }   =   x ^ { q }   -   x$ and $Q _ { a d }$ denotes the common zero of $x - a$ and $t - c ,$ may be used to define a code with different recovery sets than those considered above.

3. A bound on the minimum distance is given in [9, Theorem 3.1]

## 3 Locally recoverable codes with availability from products

## 3.1 Availability from cyclic extensions viewed as fiber products of curves

If every coordinate j has t disjoint recovery sets, then C is said to have availability t to reflect that information is more available to users in the presence of erasure. In [9], fiber products of curves are used to construct locally recoverable codes with availability. We review the construction in the case $t = 2$ below.

Suppose $X   =   Y _ { 1 } \times _ { Y } Y _ { 2 }$ where $Y _ { 1 } , \; Y _ { 2 }$ , and $Y$ are curves over a finite field F with rational, separable maps $h _ { i } : Y _ { i } \to Y$ . The $\mathbb { F } _ { q ^ { * } }$ -rational points of $X$ are $\{ ( P _ { 1 } , P _ { 2 } ) : P _ { i }$ is an $\mathbb { F } _ { q } -$ rational point on $Y _ { i } , h _ { 1 } ( P _ { 1 } ) \stackrel { \cdot } { = } h _ { 2 } ( P _ { 2 } ) \}$ . Thus, there are projection maps $g _ { i }   :   \stackrel { \cdot } { X }   \rightarrow   Y _ { i }$ defined by $g _ { i } ( P _ { 1 } , P _ { 2 } ) \: = \: P _ { i } ;$ a rational, separable map $g   :   X   \to   Y$ given by $g   =   h_{1}   \circ   g_{1}   =   h_{2}   \circ   g_{2};$ maps of function fields $h _ { i } ^ { * } : \mathbb { F } ( Y ) \to \mathbb { F } ( Y _ { 1 } )$ given by $h _ { i } ^ { * } ( f ) : = f \circ h _ { i } ;$ and primitive elements $x _ { i }$ of the extensions $\mathbb { F } \left( Y _ { i } \right) / h _ { i } ^ { * } \left( \mathbb { F } \left( Y \right) \right)$ . Let $S$ be a set of F-rational points on $Y$ , and take $\textstyle D : = \sum _ { P \in g ^ { - 1 } ( S ) } P$ . Choose an effective divisor G on $Y$ of degree $\ell < | S |$ , and take a basis $\{ \dot { f } _ { 1 } , \ldots , f _ { t } \}$ for ${ \mathcal { L } } ( G )$ . Set

$$
V := \operatorname{Span} \left\{\left(f _ {i} \circ g\right) x _ {1} ^ {* e _ {1}} x _ {2} ^ {* e _ {2}}: 1 \leq i \leq t, 0 \leq e _ {i} \leq \deg h _ {i} - 2 \right\}
$$

where $x _ { i } ^ { * } = g _ { i } ^ { * } ( x _ { i } )$ given that $g _ { i } ^ { * } : \mathbb { F } ( Y _ { i } ) \to \mathbb { F } ( X )$ for $i = 1 , 2$ . Consider

$$
\begin{array}{c} e v: V \to \mathbb {F} ^ {n} \\ f \mapsto (f (P _ {i})) _ {P _ {i} \in \mathrm{supp} D}. \end{array}
$$

Then the code $C ( D , G , g , g _ { 1 } , g _ { 2 } ) : = e v ( V )$ has length $| D | = \deg g | S |$ , dimension

$$
t \left(\deg h _ {1} - 1\right) \left(\deg h _ {2} - 1\right),
$$

and minimum distance bounded below according to [9]. For $i = 1 , 2$

$$
g _ {i} ^ {- 1} (g _ {i} (Q)) \setminus \{Q \}
$$

serves as a recovery set for $Q \in S$ . Hence, $C ( D , G , g , g _ { 1 } , g _ { 2 } )$ has locality 2. Next we apply this construction to $\tilde { S } _ { q }$ and $\tilde { R } _ { q }$

Cyclic extensions of Suzuki curves as fiber products. Because $\tilde { S } _ { q }$ is the fiber product of covers $S _ { q } \to \mathbb { P } _ { x } ^ { 1 }$ and $C _ { m } \to \mathbb { P } _ { x } ^ { 1 } ,$ we may apply the construction to obtain a code with availability 2 and localities $m - 1$ and $q - 1 !$ ; that is, every coordinate has 2 disjoint recovery sets, one of cardinality $q - 2 q _ { 0 }$ and one of cardinality $q - 1$ . To do this, consider the projection maps $g _ { 1 } \colon \tilde { S } _ { q } \to C _ { m } ,$ g ${ } _ { 2 } : \tilde { S } _ { q } \rightarrow S _ { q }$ , and $g : \tilde { S } _ { q } \to \mathbb { P } _ { x } ^ { 1 }$ . We take $S$ as in (1), D as in (2), and $G : = \alpha P _ { \infty }$ where $P _ { \infty }$ is the unique point at infinity on $\mathbb { P } _ { x } ^ { 1 }$ . Fix a basis B of ${ \mathcal { L } } ( G )$ , and

$$
V := \left\langle f y ^ {i} t ^ {j}: 0 \leq i \leq q - 2, 0 \leq j \leq m - 2, f \in \mathcal {B} \right\rangle_ {\mathbb {F} _ {q ^ {4}}}.
$$

![](data:image/jpeg;base64,/9j/4AAQSkZJRgABAQAAAQABAAD/2wBDAAIBAQEBAQIBAQECAgICAgQDAgICAgUEBAMEBgUGBgYFBgYGBwkIBgcJBwYGCAsICQoKCgoKBggLDAsKDAkKCgr/2wBDAQICAgICAgUDAwUKBwYHCgoKCgoKCgoKCgoKCgoKCgoKCgoKCgoKCgoKCgoKCgoKCgoKCgoKCgoKCgoKCgoKCgr/wAARCAEAAQMDASIAAhEBAxEB/8QAHwAAAQUBAQEBAQEAAAAAAAAAAAECAwQFBgcICQoL/8QAtRAAAgEDAwIEAwUFBAQAAAF9AQIDAAQRBRIhMUEGE1FhByJxFDKBkaEII0KxwRVS0fAkM2JyggkKFhcYGRolJicoKSo0NTY3ODk6Q0RFRkdISUpTVFVWV1hZWmNkZWZnaGlqc3R1dnd4eXqDhIWGh4iJipKTlJWWl5iZmqKjpKWmp6ipqrKztLW2t7i5usLDxMXGx8jJytLT1NXW19jZ2uHi4+Tl5ufo6erx8vP09fb3+Pn6/8QAHwEAAwEBAQEBAQEBAQAAAAAAAAECAwQFBgcICQoL/8QAtREAAgECBAQDBAcFBAQAAQJ3AAECAxEEBSExBhJBUQdhcRMiMoEIFEKRobHBCSMzUvAVYnLRChYkNOEl8RcYGRomJygpKjU2Nzg5OkNERUZHSElKU1RVVldYWVpjZGVmZ2hpanN0dXZ3eHl6goOEhYaHiImKkpOUlZaXmJmaoqOkpaanqKmqsrO0tba3uLm6wsPExcbHyMnK0tPU1dbX2Nna4uPk5ebn6Onq8vP09fb3+Pn6/9oADAMBAAIRAxEAPwD9/KKKKACiiigAooooAKKKKACiiigAooooAKKKKACvG/2k/wBuL4U/su65baJ408D/ABC1wfZRea1eeB/AGoa1BoNmWKi6vWtI38qMlXwqhpCEZtm1Sw9kr5j+F/7Q3xm/bd+I/j+y/Z58f6Z4L+Hnw78XXPhOfxL/AGImo6rr2s2oQ3hgWZxBa2sLuItzxzPMwcjylUFwD2L4AftFfDb9pvwbJ8RvhE+rXXh43RhsNY1LQrmwi1HCqWkt1uUjkkiBO3zNgUsrBS2M13VfNX7NPxk/a98R/tpfEj9nf41R+Grvwn8OvCukzaf4p0bTpLabxBd6i80iPLEzuts0MNu0bIjMrs/mDYGWNPpWgAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigDL8a+NPC3w68J6h468b63DpukaVavc6hf3BISCJRkscZP5c1+cHxX/4I4f8FBv2dfjd48/aJ/4JC/8ABRs+AbXx74huPEet/CLx54bj1DQ7jUpvnmeOV1lMAkb0h3gYHmYUY+1P26/2Zdf/AGxv2ada/Zv0X4mXPhCDxNd2MWsa7YQh7qGwju4pp0g3AqJXSMorMCq78lWA2nn9U+HP/BRPV/DD/CwfHv4dWFpJbG1k+I1j4WuzrXkldvmR2Lzm2jutv/LYyvEH+f7Pj93QBz3/AASF/a3+If7b/wCxlpnx/wDjX8NNK8M+PjrWo6D42g0aErb3N/pt1JaPLGSWYofL4BZtp3AHAr6VsNe0LVSRpetWlyRI8ZFvcq/zoQHXg9VJAI7ZGa+Pf2yv2YNb+BP7CXgH9lP9lHRvEdp4FtfHuhad8R5fDqXF1q7+F3uzJqs/7hTPNJOxzO8YLlJpjjGcYH7BGi/Cj4qf8FNPjd8aPg38MR4Y8K/DbwboPw80Oy/4Rl9ID3bq2oXsotZI43QiNrCMM6KxSNf4cUAfdlFFFABRRRQAUUUUAFFFFABRRRQAUUUUAFFFFABRRRQAUUUUAFFFFABRRRQAUUUUAFFFFABRRRQAUVwf7R/7R/ww/ZZ+GFx8Uvilf3PkfaYrLSNI0u1NzqOt6jMdtvp9jbr81zdTP8qRr7sSqqzDH/ZTi/ak1XwpqfxE/asvNP03WPE2oC80n4f6SkUsHg+w8tVisXu0XdfXRwZJ5s+V5rlIVEaBnAPVKKKKAGXMIubeS3MjoJEKl43KsuRjII6H3rzT9nP9kj4R/stXfiu/+FsviB5/G2vNrXiWbXvEt3qL3eoMio9xm4dyrMqIp24GEUAAAVB+yv8AHjxL8d4/iK/iXSLG0/4Q74q6z4XsPsKuPOtrNoxHLJvZv3h3ndjC8DAFeq0AFFFfOHw//aJ+Kv7Pvx3i/Zk/bM1uG+s/F2sTD4P/ABWjso7W110uzSLoV+kYWK11WJMiIqFjvYo90YWVJIqAPo+iiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACvnb9rf9tzVfhN8dvh5+xj8B/DWn6/8Wvid9putNt9XkcafoGj2ozdateiIiR41+5HChRppDt3oAWH0TXx/wDEz9l34t/D/wD4KyWf/BQ3wn8Lb/x9omo/BaTwVPpWj6pYW99ol2l+LpJ1W+uII3glQsjFHLqyj5CGyAC58M/26PjNYf8ABQXxj+xJ8arHwNc6H4C+FsPjLxJ8TdIafSoLYXM4jgtpbS4muFtyFjuJGc3LBlVWwnIr3j4FftN/Ar9pm18QX3wK+IVt4it/C+vPout3FpbzJHBerFFM0StIiiUeXNG29CyHdw2QcfHf/BEi+8WftA/FP9p79vn4g6PZ2moeP/jBJ4b0eKxvTcww6XoMIso0imZE8xfNabLBVBZSQK9C/wCCWX/JWf2tP+zm9T/9Nem0AfX1FFFABXB/tH/tH/DD9ln4YXHxS+KV/c+R9pistI0jS7U3Oo63qMx22+n2NuvzXN1M/wAqRr7sSqqzA/aP/aP+GH7LPwwuPil8Ur+58j7TFZaRpGl2pudR1vUZjtt9PsbdfmubqZ/lSNfdiVVWYeX/ALOH7OHxP8f/ABPt/wBtX9tWwtv+E/8As0sPgLwFb3QudO+HGnTDDwQuPludTmTAur4Dn/UQ7YVJlAD9nD9nD4n+P/ifb/tq/tq2Ft/wn/2aWHwF4Ct7oXOnfDjTphh4IXHy3OpzJgXV8Bz/AKiHbCpMv0ZRRQAUUUUAfOX/AATn/wBT8c/+zjvFX/ocFfRtfOX/AATn/wBT8c/+zjvFX/ocFfRtABXLfGr4K/DD9oj4Yav8G/jJ4Rttc8O65beTf2FyWXowZJI3Uh4pY3VZI5UKvG6K6MrKCOpooA+Yfgr8avif+yj8T9I/Y6/bF8XXOuWGuXP2T4OfGPUQq/8ACS4UsujaqygJFrMaKdkmFS/RC6BZllir6erlvjV8Ffhh+0R8MNX+Dfxk8I22ueHdctvJv7C5LL0YMkkbqQ8UsbqskcqFXjdFdGVlBHhXwV+NXxP/AGUfifpH7HX7Yvi651yw1y5+yfBz4x6iFX/hJcKWXRtVZQEi1mNFOyTCpfohdAsyyxUAfT1FFFAGJ8SviR4G+D3w+1r4q/E3xNa6N4e8O6ZNqGtareybYrW2iQvJIx9AoPA5PQZNfJHxR/4K4eKfhP8Ask2v7eniH9kHVW+F2t3lnD4ViXxTGniXUY72ZYbG5fTZIFhgind4yB9reZUkVmiB3KvR/wDBbj9nD4w/tY/8E0/iJ8EPgbp1xqGuaklhO+j2cgWbVLS3voJ7m1jJIBkeGNwqkjccL3r58/bp+J+hftSftMfse/8ABPT4e/C7xdpfhq68e2/jDxA3ibwtdaOo07w9aeeloLa8SKdgJWhDP5flZChXZgwUA+z/AIk/t6/syfB211DUPid4v1bTLLRJY4fEetQ+DtVvNK0SZgpaK81G2tpLS1ZN67/NlUR7hvK5FeuaZqena1ptvrGj38N3aXcCTWt1bSh45o2AZXRlyGUgggjgg18Pftnav+zV4W8G/FH9gv8AZ18a+BvBvin4svfah8VfEPiXxLGll4cTUogl3fzR3E2ZbyWEEw2cW0E7ZH8uM7n+qf2UvDnw58H/ALMXw88J/B/WbrUvCmm+CtMtfDeo3ysJrqxS1jWCVwwB3NGFY5A69BQB39FFFABRRRQAUUUUAFFFFABRRRQAV84P/wAE4PD+lfEf4ofEX4dftRfFbwpL8YNSivfGlpo+rafKN0cHkBLSW7spprJPLyP3TqylsoyELt+j6KAOD+DX7MvwK+APwJ079mj4XfDfT7HwRpmnPZRaDPH9pinikLGXz/O3GdpGZ2kaQsXLsWJyai+GP7JP7KnwT8STeMvgz+zL8PvCOr3O/wC0ar4Y8GWNhcy7wA+6WCJWbdgZyecDNeg0UAFcH+0f+0f8MP2WfhhcfFL4pX9z5H2mKy0jSNLtTc6jreozHbb6fY26/Nc3Uz/Kka+7EqqswP2j/wBo/wCGH7LPwwuPil8Ur+58j7TFZaRpGl2pudR1vUZjtt9PsbdfmubqZ/lSNfdiVVWYeX/s4fs4fE/x/wDE+3/bV/bVsLb/AIT/AOzSw+AvAVvdC5074cadMMPBC4+W51OZMC6vgOf9RDthUmUAP2cP2cPif4/+J9v+2r+2rYW3/Cf/AGaWHwF4Ct7oXOnfDjTphh4IXHy3OpzJgXV8Bz/qIdsKky/RlFFABRRRQAUUUUAfOX/BOf8A1Pxz/wCzjvFX/ocFfRtfOX/BOf8A1Pxz/wCzjvFX/ocFfRtABRRRQAVy3xq+Cvww/aI+GGr/AAb+MnhG21zw7rlt5N/YXJZejBkkjdSHiljdVkjlQq8boroysoI6migD5h+Cvxq+J/7KPxP0j9jr9sXxdc65Ya5c/ZPg58Y9RCr/AMJLhSy6NqrKAkWsxop2SYVL9ELoFmWWKvp6uW+NXwV+GH7RHww1f4N/GTwjba54d1y28m/sLksvRgySRupDxSxuqyRyoVeN0V0ZWUEeFfBX41fE/wDZR+J+kfsdfti+LrnXLDXLn7J8HPjHqIVf+ElwpZdG1VlASLWY0U7JMKl+iF0CzLLFQB337VfwA+OHxq1fwD4i+CX7UOp/DubwX4qGr6ppsGnNc2PiiERsgsL1I54JDBuIYhXwccqeCuB8Jv2ItR0z9q+9/be/aG+KkXjX4hDwyfDnhhNL0E6XpPhvSmlEssVrbPPcSNNNIAZZ5JmLBQqrGvy17/RQB4p43/4Jr/8ABOj4meLtQ+IHxI/YE+CniDXtXumudV1vW/hXpF3eXs7HLSyzS27PI5PVmJJ9a9i0fR9I8PaTa6BoGl21jYWNulvZWVnAsUNvEihUjRFAVFVQAFAAAAAqzRQAUUUUAFFFFABRRRQAUUUUAFFFFABRRRQAVwf7R/7R/wAMP2WfhhcfFL4pX9z5H2mKy0jSNLtTc6jreozHbb6fY26/Nc3Uz/Kka+7EqqswP2j/ANo/4Yfss/DC4+KXxSv7nyPtMVlpGkaXam51HW9RmO230+xt1+a5upn+VI192JVVZh5f+zh+zh8T/H/xPt/21f21bC2/4T/7NLD4C8BW90LnTvhxp0ww8ELj5bnU5kwLq+A5/wBRDthUmUAP2cP2cPif4/8Aifb/ALav7athbf8ACf8A2aWHwF4Ct7oXOnfDjTphh4IXHy3OpzJgXV8Bz/qIdsKky/RlFFABRRRQAUUUUAFFFFAHzl/wTn/1Pxz/AOzjvFX/AKHBX0bXzl/wTn/1Pxz/AOzjvFX/AKHBX0bQAUUUUAFFFFABXLfGr4K/DD9oj4Yav8G/jJ4Rttc8O65beTf2FyWXowZJI3Uh4pY3VZI5UKvG6K6MrKCOpooA+Yfgr8avif8Aso/E/SP2Ov2xfF1zrlhrlz9k+Dnxj1EKv/CS4UsujaqygJFrMaKdkmFS/RC6BZllir6erlvjV8Ffhh+0R8MNX+Dfxk8I22ueHdctvJv7C5LL0YMkkbqQ8UsbqskcqFXjdFdGVlBHhXwV+NXxP/ZR+J+kfsdfti+LrnXLDXLn7J8HPjHqIVf+ElwpZdG1VlASLWY0U7JMKl+iF0CzLLFQB9PUUUUAFFFFABRRRQAUUUUAFFFFABRRRQAV5Z8UP2zP2ffhZ+z1Y/tNXvjRdZ8Na5Da/wDCIr4ciN5d+Jbm6A+yWWnwJ81zczsQqRrz1LbVVmX1Ovzo/YE/Y1/4KB/Aj4k+GfHf7V/wD8AeMv8AhHLmXSvBc2j/ABYlK+A9Ku52a6ubPTptJSK4vZTI7T3TXKytCFhiCqGSUA+i/wBnD9nD4n+P/ifb/tq/tq2Ft/wn/wBmlh8BeAre6Fzp3w406YYeCFx8tzqcyYF1fAc/6iHbCpMv0ZRRQAUUUUAFFFFABRRRQAUUUUAfOX/BOf8A1Pxz/wCzjvFX/ocFfRtfOX/BOf8A1Pxz/wCzjvFX/ocFfRtABRRRQAUUUUAFFFFABXLfGr4K/DD9oj4Yav8ABv4yeEbbXPDuuW3k39hcll6MGSSN1IeKWN1WSOVCrxuiujKygjqaKAPmH4K/Gr4n/so/E/SP2Ov2xfF1zrlhrlz9k+Dnxj1EKv8AwkuFLLo2qsoCRazGinZJhUv0QugWZZYq+nq8+/ap8G/Bbx9+z54q8MftDeB7vxH4Pm0tn1nStN0W71C7kVCHR7aCyR7lrhHVXjaBfNSRFdCGUEfFn7GPxK/ap8c/8FA/A3wq+Lnw4+K+teBfh18O/FZ8LfFfxz8PdV0dtVa7uNESyg1QXFvHCdUt7eDUIvtCHbcxzmQKkjSoAD9FKKKKACiiigAooooAK4P4z/tI/Cn4E3ej6H411S9udc8RzSReHPDGg6VPqGp6o0ahpDDbW6s5jQEF5WCxRhgXdQRXeV+Y37c37Qv7VH/BOD/gqxqv7bfiT9izx98ZPg14p+E+n+HLfWPhvp32+/8AB0tvdST3INv0WOZnV2ZmjV8R/PmMrQB9lWn/AAUA+A8PxU8M/AXxtZeJvCvj3xhqBtfD3g3xJ4fkgvb1BDLNJcxMheGW3jSF/MljkZYyUV9rOgb2+vj/APYz/an/AGHv+Cunjnwn+2f+z94q1CfWPg8NX0m78M6/ov2PUtHutSigRvtEbE7T5du4Uxs6NvcbsoQPsCgAooooAKKKKACiiigAooooAKKKKACiiigD5y/4Jz/6n45/9nHeKv8A0OCvo2vnL/gnP/qfjn/2cd4q/wDQ4K639un9s/4b/sF/s66p+0B8R9Pu9S8m6t9O8P8Ah7TSPteuarcyCK1sYM8b5JCBuPCqGY5C4oA9gor4u+OH7a/7cH7L/iv4DeFPih4D+GviDWfjl8QLTw5J4W0Fb6zm8Lh0Nxcy/apJphqSw26SKziG1G8K23ado+0aACiiigAooooAKKKKACiiua+Mnxa8DfAX4T+JPjX8TdYXT/D3hTRLnVdZvGGfKt4I2kcgdzhTgdyQKAN/UNQsNKsZtT1S9htra3jMk9xcShEjQDJZmPAAHc1i+EPiz8K/iDp15q/gL4meH9ctNPYrf3WkazBcx2xAJIkaNyEwATzjpXwx/wAE+fh14m/4Ky+FYP8Agot+3loDal4P8R38tx8FPgnqZ8zQ9E0iORkg1K9tT+7v9Qm2mQSTK6xKV8oLnI7v/gqp/wAE6dG/aN/Z6tPC37O/hEeFPFN14l0XStR1XwVCunT3Ph2fUII9TsrloNnnWn2YySNE+VBiBAz1APsm3uLe7t0urWdJYpUDxyRsGV1IyCCOCCO9PqDTNNstH0230jTbdYba1gSG3iQYCIoCqo9gABU9ABRRRQBmeNfE1v4L8Hat4xu7eWaLSdMnvJIYIy7usUbOVVRksxC4AHJNfKn7G/8AwUEtNA/Zg8Jwft16Z4u8CfE6TSxca1oXiXwvfS3F68rtJGbRreGRLvMboPKhLyRH926I6la+vqKAPhj9nPwrZ/steIf2qv8Agrp8YvhtqPg3SPHdvb6xZ+EbizEGpDRtF091W7uoP+WN3duZJPKfDopjEm1yyro6R+2B+2ofHP7NmmeJ4/Bdpe/H7Up77V/BUGh3Dy+F9Ct7E6hIVu/tP7+4WMw28kjRqhknDIiBMN9h+NPBnhT4jeENU8AeOtAtdV0XW9PmsdW0y9iDw3VtKhSSJ1PVWViCPevkv4QfsH/EH4cf8FKdO/aDt/Bscfw58JfCu48I+DpNW+Jeo6xqFvJLcxyPMIbxXEERhghhWNJc8MzZJ5APseiiigAooooAKKKKACiiigAooooAKKKKAPnL/gnP/qfjn/2cd4q/9DgrB/4Kq/se/EL9qvwz8I/FPw30VNdu/hP8aNF8a3nhGS9itzrtpas6y28UkzLEswWTenmMiEptLLnNb3/BOf8A1Pxz/wCzjvFX/ocFdF8df2JPDPxq/aG8HftS2Hxi8beE/GHgbRb/AEvRJ/D97ayWZgvNvnNLaXlvPC8mFwr7QQDznAwAfKq+LfiJ+17/AMF8vBfhrxv8L5vDOg/s+fCDUPEkOl6jqlvdXi6nrMi2UT3ItXlgifyI5WRElkO35iVLFF/RWvKf2bf2PPhV+zLrHivxx4c1DWvEHjHx5qMd9428c+KrxLjVNZljTZCsjRRxxRRRJ8scEMccSDO1ASSfVqACiiigAooooAKKKKACvl//AILS/A34m/tI/wDBLL42fBn4OWM934k1fwXK2l2FrzLeNDJHO1ugH3mkSJkC9ywHevqCigD59/4JT/EH4Y/Ej/gnL8GdZ+EmoW82lWfw80rTZYIGG6yu7a1jguLaVescscqOrIQCCDkV2OifHPUfiZ+0Afh/8I5bDUvC/he1uR498QIplij1E7Vt9Mt5VbaZ0/eSzj5vKURI2Gk4oeJf2Av2N/FnjPUfiDqn7P2hRatrU3na9caYklkuryd3vI7d0jvGPAJmVyQMGvTvCPg7wj8P/DVn4M8B+FtO0TR9OhEOn6VpFjHbW1tGOiRxRgKi+wAFAHzx4W/4KVaJ4/8AjH4s+AvgP9nTxzfeI/C2g22uyC5W0tbR9KuGnWG5lnlmAtmc27FYHHnMkkb7MeYY6fgL/gqv8LPiJ8OPhf8AG/R/g343tfA/xX8T6f4d8Pa/qltawuNRvA4iX7N55lkhWSN43mQFcruTzI/3g3Y/2G/EllD+0fr+k/GgQeKfj07R2OvHQ8/8I3ax6Smn2kITzgbjyT5sud0e5pSMLjJxviD/AME4rjVPBnwB8DfCz4wR+HbH4DW7R6VBdeHvtcd3KNKbTob0IJ4/LuYA8ksRYyIHkJZGwKAPqGiq+k2H9laXbaWb2e5+zW6RfaLqTfLLtUDe7fxMcZJ7kmigCxRRRQAUUUUAFcz8IPjL8L/j54Dtfib8HvGdnr2h3cs0Md7aFhsmhkaKaGRHAeKWORHR4nVXRlKsoIIrpq+Zvjt8Cfil+zl8UtU/bP8A2MPDD6rd6q6zfFz4R2sqxQ+NIUUL/aNjuIjt9bijUBXJVLxEWCYhhDNEAfTNFch8Cfjt8Lf2lPhbpfxj+DnidNV0LVUbypfKaKa3mRik1tPC4ElvcRSK0ckMgV43RlZQQRXX0AFFFFABRRRQAUUUUAFFFFAHzl/wTn/1Pxz/AOzjvFX/AKHBX0bXzl/wTn/1Pxz/AOzjvFX/AKHBX0bQAUUUUAFFFFABRRRQAUUUUAFeYaD+1T4J8aftM6n+zL8PdC1PX7zwzpZufHHiLTUjOmeG7l/La2065mZwWvJo3aUQRB2jjVXlEayxF/Ov2gP2gPil8bvilf8A7Ff7FevrY+IbFY1+KXxSW3Se18AWsqBlghVgY7nWZo2DQ27ZSBGW4nG3yop/YP2f/wBn/wCFv7MfwtsPhD8IdAax0qxaSaWW4uHnutQupXMk95dTyEyXNzNIzSSTSEu7sSTQB2lFFFABRRRQAUUUUAFFFFABRRRQAUUUUAfM3x2+BPxS/Zy+KWqftn/sYeGH1W71V1m+LnwjtZVih8aQooX+0bHcRHb63FGoCuSqXiIsExDCGaL2r4E/Hb4W/tKfC3S/jH8HPE6aroWqo3lS+U0U1vMjFJraeFwJLe4ikVo5IZArxujKyggiuvr5m+O3wJ+KX7OXxS1T9s/9jDww+q3equs3xc+EdrKsUPjSFFC/2jY7iI7fW4o1AVyVS8RFgmIYQzRAH0zRXIfAn47fC39pT4W6X8Y/g54nTVdC1VG8qXymimt5kYpNbTwuBJb3EUitHJDIFeN0ZWUEEV19ABRRRQAUUUUAFFFFAHzl/wAE5/8AU/HP/s47xV/6HBX0bXzl/wAE5/8AU/HP/s47xV/6HBX0bQAUUUUAFFFFABRRRQAV81/tAftAfFL43fFK/wD2K/2K9fWx8Q2Kxr8Uviktuk9r4AtZUDLBCrAx3OszRsGht2ykCMtxONvlRTn7QH7QHxS+N3xSv/2K/wBivX1sfENisa/FL4pLbpPa+ALWVAywQqwMdzrM0bBobdspAjLcTjb5UU/sH7P/AOz/APC39mP4W2Hwh+EOgNY6VYtJNLLcXDz3WoXUrmSe8up5CZLm5mkZpJJpCXd2JJoAP2f/ANn/AOFv7MfwtsPhD8IdAax0qxaSaWW4uHnutQupXMk95dTyEyXNzNIzSSTSEu7sSTXaUUUAFFFFABRRRQAUUUUAFFFFABRRRQAUUUUAFFFFAHzN8dvgT8Uv2cvilqn7Z/7GHhh9Vu9VdZvi58I7WVYofGkKKF/tGx3ER2+txRqArkql4iLBMQwhmi9q+BPx2+Fv7Snwt0v4x/BzxOmq6FqqP5UvlNFLbzIxSa2nhcCS3uIpFeOSGQK8boysoIIrr6+Zvjt8Cfil+zn8UtU/bP8A2MPDD6rd6q6zfFz4R2sqxQ+NIUUL/aFjuIjt9bijUBXJVLxEWCYhhDNCAfTNFch8Cfjt8Lf2lPhbpfxj+DnidNV0LVUfypfKaKa3mRik1tPC4ElvcRSK8ckMiq8boysoIIrr6ACiiigAooooA+cv+Cc/+p+Of/Zx3ir/ANDgr6Nr5y/4Jz/6n45/9nHeKv8A0OCvo2gAooooAKKKKACvmv8AaA/aA+KXxu+KV/8AsV/sV6+tj4hsVjX4o/FJbdZ7XwBayoHEEIYGO51maNg0Nu2UgRluJxt8qKc/aA/aA+KXxu+KV/8AsWfsWa+tj4hsVjX4pfFJbdJ7XwBayoHW3hVwY7nWZo2DQ27ZS3RluJxt8qKf2D9n/wDZ/wDhb+zH8LbD4Q/CHQGsdJsWkllluLh57q/upXMk95dTyEyXNzNIzSSTSEu7sSTQAfs//s//AAt/Zj+Fth8IfhDoDWOlWLSTSy3Fw891qF1K5knvLqeQmS5uZpGaSSaQl3diSa7SiigAooooAKKKKACiiigAooooAKKKKACiiigAr4m8S/Fj9oHWv+C6nhr9nTwD+0J4kfwDo3wdvPF3xB8GTWmnHT4ppZhZafFG6Wi3KlnEk7eZM+SBjavFfbNfmn+wp8HPhB+17+2x+1v/AMFCvj7p1rr/AIUsfHg8D+G9K1xFn0kWOgWqpPdyQODHORM8pUuGEZDsoDHIAP0sor4a/wCCEut+IvEH7B1x4lh1620uHx3448VeJfhZ4e1HMq6P4cl1F0sUjthIj/ZEO1giMihZlVWXK16P+wJ8bvj98Sfjt+0X8L/jn8RrPxGnw5+I9hpHh+aw0CLT4oLeXR7S6dFjVncgyzOR5kkjAYG7AoA+nqKKKAPmb47fAn4pfs5/FLVP2z/2MPDD6rd6q6zfFz4R2sqxQ+NIUUL/AGhY7iI7fW4o1AVyVS8RFgmIYQzQ+1fAn47fC39pT4W6X8Y/g54nTVdC1VH8qXymimt5kYpNbTwuBJb3EUivHJDIqvG6MrKCCK6+vmb47fAn4pfs5/FLVP2z/wBjDww+q3equs3xc+EdrKsUPjSFFC/2hY7iI7fW4o1AVyVS8RFgmIYQzQgH0zRXIfAn47fC39pT4W6X8Y/g54nTVdC1VH8qXymimt5kYpNbTwuBJb3EUivHJDIqvG6MrKCCK6+gAooooA+cv+Cc/wDqfjn/ANnHeKv/AEOCvo2vnL/gnP8A6n45/wDZx3ir/wBDgr6NoAKKKKACvmv9oD9oD4pfG74pX/7Fn7FmvrY+IbFY1+KXxSW3Se18AWsqB1t4VcGO51maNg0Nu2Ut0ZbicbfKinP2gP2gPil8bvilf/sWfsWa+tj4hsVjX4pfFJbdJ7XwBayoHW3hVwY7nWZo2DQ27ZS3RluJxt8qKf2D9n/9n/4W/sx/C2w+EPwh0BrHSbFpJZZbi4ee6v7qVzJPeXU8hMlzczSM0kk0hLu7Ek0AH7P/AOz/APC39mP4W2Hwh+EOgNY6TYtJLLLcXDz3V/dSuZJ7y6nkJkubmaRmkkmkJd3Ykmu0oooA4z9ozXPB/hf4A+NPFPxBvr220LSvC99e6vPp2r3FhOlvDA8khjuLaSOaFtqnDxurDsRXx9/wQr+Mfgjwn+wJ8NPCfxx/ai06/wDiT8QJLnxEvh3xd8Qhe60qahNJcWlqqXc73LhbTySqnJIyeck10f8AwX18e63of/BOHxB8HfBtw6eIfjD4h0j4faEsR+d5NUvI4Zdv0txOT7V5p/wUM+CHw++G3g39kj/glZ+zvoFvpR1r4u6Pqg+xrtls9I8PqL6+v3f73msyRBpCdzNK2ScmgD9HKK8M8V+Jvjv8fPiP4OX4C+Mb7Q/hLrPhO91LUfiP4ak0035vxLElpbJb6na3AaB4zNJ5iw8lE+YLw/If8Evv2k/jV+0F4c+Kmg/FzxBD4ptPh58WtU8K+FfiLb6bFaL4psbYR/6Q0cCrCZYpGkgkeFViZoiVVeRQB9RUUUUAFFFFABRRRQAUUUUAFFFFADJ4jPA8IlaMuhUOmNy5HUZyM14V+z//AME9fgn+z7+xhqf7D+javruq+GddtdZi1/VdSvEXUb99Ukmku5nliRFEhM7AMFGAF64594ooA8V/Za/Y08Ofsi+B7bQ/CHivVPFeo6N4Rs/Dnh258SyW9utpplkhFtZJ9kt1WJCx3yS+W7u7bjkKiLwv7F37PP7WvwV/aU+NPxR+LnhD4dRaF8W/Glvr0R8OePL+9u9LEGl29ksJim0m3Sbc1uGLeYm0P0Yrz9SUUAFFFFABRRRQB8zfHb4E/FL9nP4pap+2f+xh4YfVbvVXWb4ufCO1lWKHxpCihf7QsdxEdvrcUagK5KpeIiwTEMIZoeh/Zj/bm8Dftb/GbX/C3wauLPUPCWjfDzQNfXWGMkd4bzUb/WrSWxmt3UNbSWzaO6SxyASpK7IyqYzn034y/CHRPjf4N/4QTxH4s8V6PYyXcc1zP4N8W3uh3kypn919ssZYriJGJG7ypEY4A3YyD59+y1/wT2/Zg/Y2+Ifjn4q/Anw94it9f+JP2FvGuq+I/HWra5capJZ+f5EskupXM8hkAuZRu3ZIIHagD2yuL+Pfxu0n9nvwC3xN8SeB/E+taRaXSLrMnhTRzfz6XakMZL6S3RvOlgj2jeIEllAbcI2UMV7SigD4d/4J6/tjfBu58dfEH4YfDm5v/HOr+Nvjz4h1qwXwXAl5b2WhzyQCPWbu4LrDb2bkERsXMk5VxBHMUcL9xV5t+zl+yD+zd+ySniyP9nb4U6f4XXxx4rufEfiYWLSN9r1CfG9x5jN5UYx8kEe2KPLbEXcc+k0AQan/AGl/Ztx/Ywg+2eQ/2T7Vu8rzdp279vO3OM45xXxh4I/bw+LH/BQj4T+BPh3+xoY/DniXxj8P9D8Q/FXx/Ei3dn8NrfUrCG7Flblxsu9Ykjm/cQMCsKFbm4XaYoZ/tO5gS6t5LWRnVZEKs0chRgCMcMpBB9xyK+bv2Xv+CT/7JP7F9loOk/sxXfxK8Kad4fvDcW2g23xj8Q3GkXDNnzRNplxeyWT+YTuY+QGzypU0Aewfs/8A7P8A8Lf2Y/hbYfCH4Q6A1jpNi0ksstxcPPdX91K5knvLqeQmS5uZpGaSSaQl3diSa7SiigAooooA4L40fszfBj9oPxF4H8UfFrwo2q3Xw68Vx+JPCeb+aKO11KOKSKOdkjdVm2rIxCyBgDg4yKzPjr+x38Bv2jPGXh34ifEzw5ftrvhe1vbPSNX0fXbvT7hLO8RUu7VpLaRGeGVUQMhP8PBGTn1CigD5k/b1+FP7e/jHwL4c+Dv7BWkfCnTfCqw+R4ui8YeL9V0WeWyRQken2babYXDQQsoxJIrxyBBsTZkuOs/YR+HP7Ufwx+F974a/ad0D4X+H5La+jt/CXhH4Py3MmiaLpcVvGixo91a28pkaUSuwKFRuUA9a9vooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKAI57q1tVDXVzHGCcAyOBn86SC9s7olbW7ikIHIjkBx+VfMv/BVr4I/Bj4t/Abw9qPxV+EfhjxNPpXxN8KR6ZN4g0G3vWtUuNf0+KdIzMjbFljJRwMB1O1sjivk3xj+wx+wxp/7MXi3x3H+zH8OdJ8RWP7WEtloerWHhm0sboRDxTFamzSeFEkSEWfmr5asFRFLADYCAD9EP2ofiD8VfhV8EvEPxG+Efhrw/qeo6HpF3qE0fiPU57eGOKC3klLKsMMjTNlFHllogQxPmDGDT/Yt+NPib9o/9kX4Z/H7xnp1jaav408DaZrWp2umRulvFPc2ySusQdnYIGYgBmY46k14T8LPE/wATPG37AfxF8J/C/wANeKvippt7qvibw38MNSh1qykutR0Uxyw2lzJd6ndwC5gSQvCtwZHkljjjk/ebi59M/wCCafhf4rfDX9if4bfBf4z/AAc1rwb4g8EeCNK0TUrXVtR026W5nt7SOOSSF7C7uFMe5SAXKMf7tAHu9FFFABRRRQAUUUUAFFFFABRRRQAUUUUAFFFFABRRRQAUUUUAFFFFABRRRQAUUUUAFFFFABRRRQAUUUUAeD/8FGWUfs96WpYAv8UvBaqM9T/wken8CvJf2Vf2Dv2MPjpqXxE+Knxr/Zo8HeMPEFv8dvFzR3nijRo9QETpqcwRhFOGjVgpGDtzgjmvpr40fsx/s2ftIwafa/tEfs9+B/HsWkyvJpUfjTwnZ6qtm7Y3NELmN/LJ2rkrjO0elQfBf9k/9ln9m+91DUv2eP2avAHgK41ZFXVbjwX4NsdLe8VSSola2iQyAEkjdnGaAL/xF+LHw/8AgjpulaXqdleyT6g5tNA8P+HtHlu7q58uPcyxQQKSsaIMs52xoMbmXIzwPwG/4KG/ss/tK+M9I8DfCHxlqV/d+INDu9W0Ka58OXlrDfW9pNHBdmN5ol+aGWVI3Bx8+5QWKOF1P2l9C/az1nUNJi/Zrj+Hws59J1Sy1268Wtdx31pNPCiWlxaSwI67I5A0ksbLmQJGqsmS68n/AME+P2e/j5+zD8C/A3wB+IJ8LWWh+BfBkeksmgajNfS65qO9Wl1CR5reH7OpIkYRDeWa4Ysw2AMAfQ1FFFABRRRQAUUUUAFFFFABRRRQAUUUUAFFFFABRRRQB//Z)

Fig. 1. Cyclic extension of Suzuki curve viewed as a fiber product

Theorem 2. Suppose $C ( D , G , g , g _ { 1 } , g _ { 2 } )$ is constructed as above. Then the code $C ( D , G , g , g _ { 1 } , g _ { 2 } )$ is an $[ n , k , d ]$ code over $\mathbb { F } _ { q ^ { 4 } }$ with availability 2 and recovery sets for each coordinate of sizes $q - 2 q _ { 0 }$ and $q - 1$ , where

$$
\begin{array}{c} n = (q - 2 q _ {0} + 1) \left(q ^ {4} + 2 q _ {0} q ^ {2} (q - 1) - q ^ {2}\right), \\ k = (q - 2 q _ {0}) (\alpha + 1) (q - 1), \end{array}
$$

and

$$
d \geq n - \left(\alpha m q + (q - 2) m \left(q + q _ {0}\right) + (m - 2) q ^ {2}\right).
$$

Proof. The length and dimension can be verified directly by applying [9, Theorem 3.1]. To determine the minimum distance $d ,$ we use the fact that $d \geq$ $n { - } w t ( e v ( h ) ) { \geq } n { - } \operatorname { d e g } ( h ) _ { 0 }$ where $h   =   f y ^ { q - 2 } t ^ { m - 2 }$ and $f   \in   \mathcal { L } ( \alpha P _ { \infty } )$ . Then $( h ) \mathop { = } _ { \sim } ( f ) + ( q - 2 ) ( y ) + ( m - 2 ) ( t )$ . Note that when considered as a functions on $\tilde { S } _ { q } , \operatorname { d e g } ( f ) _ { 0 } \leq \alpha m q$ , deg $\mathfrak { i } ( y ) _ { 0 } \leq m ( q + q _ { 0 } )$ , and deg $( t ) _ { 0 } \leq q ^ { 2 }$ . Putting this together, we conclude that d $i \geq n - \big ( \alpha m q + ( q - 2 ) m ( q + q _ { 0 } ) + ( m - 2 ) q ^ { 2 } \big )$ , which coincides with that given in [9, Theorem 3.1].

We claim that

$$
R ^ {(1)} := g _ {2} ^ {- 1} \left(g _ {2} \left(P _ {a b c}\right)\right) \setminus \left\{P _ {a b c} \right\} = \left\{P _ {a b ^ {\prime} c}: b ^ {\prime} \in \mathbb {F} _ {q ^ {4}} \setminus \{b \} \right\}
$$

and

$$
R ^ {(2)} := g _ {1} ^ {- 1} \left(g _ {1} \left(P _ {a b c}\right)\right) \setminus \left\{P _ {a b c} \right\} = \left\{P _ {a b c ^ {\prime}}: c ^ {\prime} \in \mathbb {F} _ {q ^ {4}} \setminus \{c \} \right\}
$$

are recovery sets for the position corresponding to $P _ { a b c }$ . Suppose $f \in V$ . Then $\begin{array} { r } { f ( x , y , t ) = \sum _ { i = 0 } ^ { m - 2 } \sum _ { j = 1 } ^ { M } \tilde { a } _ { i j } f _ { j } ^ { * } t ^ { i } } \end{array}$ . Notice that $f ( a , b , T ) \in \mathbb { F } _ { q } \left[ T \right]$ and the degree is bounded by $\deg _ { T } { \check { f } } ( a , b , T ) \leq m - 2 .$ . Hence, $f ( a , b , c )$ can be recovered using the $m - 1$ interpolation points: $P _ { a b c ^ { \prime } } \in R$ . As a result, $f \left( P _ { a b c } \right)$ may be recovered using only elements of R.

Observe that the functions in the set V are modified from the construction in Section 2 in order to obtain multiple recovery sets for each position, thus impacting the dimension of the code.

One might compare this with the code found in [9, Theorem 6.1], which has availability 2 with recovery sets of size $q   -   1$ , length $n = q ( q   -   1 ) ( q ^ { 2 }   +   2 q q _ { 0 }   +   q   +   1 )$ and dimension $k = ( q - 1 ) ( q - 2 ) ( q ^ { 2 } + 2 q q _ { 0 } + q + 1 )$ . Notice that the new codes defined using $\tilde { S } _ { q }$ give the option of using a smaller recovery set (cardinality $q - 2 q _ { 0 }$ compared with $q - 1 )$

Cyclic extensions of Ree curves as fiber products. Because $\tilde { R } _ { q }$ is the fiber product of $R _ { q } \to \mathbb { P } _ { x } ^ { 1 }$ and $C _ { m } \to \mathbb { P } _ { x } ^ { 1 }$ , we may apply this construction to obtain a code with availabili $\mathrm { ~ t y ~ 2 ~ }$ and localities $m - 1$ and $q - 1$ ; that is, every coordinate has 2 disjoint recovery sets, one of cardinality $q - 3 q _ { 0 }$ and one of cardinality $q - 1$ . To do this, consider the projection maps $g _ { 1 } \colon \tilde { R } _ { q } \to C _ { m } , \: g _ { 2 } \colon \tilde { R } _ { q } \to R _ { q } ,$ and $g : \tilde { R } _ { q } \to \mathbb { P } _ { x } ^ { 1 }$ . We take S as in (5), D as in (6), and G is a divisor on $\mathbb { P } _ { x } ^ { 1 }$ . Fix a basis B of ${ \mathcal { L } } ( G )$ , and

$$
V := \left\langle f y ^ {i} t ^ {j}: 0 \leq i \leq q - 2, 0 \leq j \leq q - 3 q _ {0} - 1, f \in \mathcal {B} \right\rangle_ {\mathbb {F} _ {q ^ {6}}}.
$$

![](data:image/jpeg;base64,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)

Fig. 2. Cyclic extension of Ree curve viewed as a fiber product

Proposition 2. The code $C ( D , G , g , g _ { 1 } , g _ { 2 } )$ constructed as above is a code with parameters $[ q ^ { 7 } - q ^ { 6 } + q ^ { 4 } - q ^ { 3 } , \dot { \ell } ( G ) ( q - 1 ) ( q - 3 q _ { 0 } ) ]$ code over $\mathbb { F } _ { q ^ { 6 } }$ with availability 2 and recovery sets for each coordinate of sizes $q - 3 q _ { 0 }$ and $q - 1$

Proof. The proof is similar to that of Theorem 2.

Remark 3. 1. As noted in Remark 2, the explicit construction for codes from the Ree curve depends on that of bases for certain Riemann-Roch spaces. We provide an alternate LRC with availability construction from the Ree curve in the Section 4. There, we see codes with more accessible parameters due to choosing functions to evaluate carefully, rather than beginning with an entire Riemann-Roch space which is difficult to describe.

2. A bound on the minimum distance is given in [9, Theorem 3.1].

Observe the functions in the set V are modified from the construction in Section 2 in order to obtain multiple recovery sets for each position, thus impacting the dimension of the code.

## 3.2 Availability from products of codes

We may also take products of codes themselves to obtain LRCs with availability, as detailed below. We begin with the simplest definition, the product of two codes, $C _ { 1 }$ and $C _ { 2 }$ , which may be generalized to more factors. Examples of this construction may be found in the next section.

Definition 1. Let $C _ { 1 }$ be an $[ n _ { 1 } , k _ { 1 } , d _ { 1 } ]$ code and $C _ { 2 }$ be an $[ n _ { 2 } , k _ { 2 } , d _ { 2 } ]$ code over the same alphabet F. The product code of $C _ { 1 }$ and $C _ { 2 }$ is defined by assigning symbols from $\mathbb { F }$ to the pairs $( i , j ) \in [ n _ { 1 } ] \times [ n _ { 2 } ]$ such that the symbols assigned in $[ n _ { 1 } ] \times \{ j \}$ , for $j   \in   [ n _ { 2 } ]$ are a codeword in $C _ { 1 }$ and $\{ i \} \times [ n _ { 2 } ] { \it ~ f o r ~ } i \in [ n _ { 1 } ]$ are a codeword in $C _ { 2 } ;$ that is,

$$
C _ {1} \times C _ {2} := \left\{\left(a _ {i} b _ {j}\right) \in \mathbb {F} ^ {[ n _ {1} ] \times [ n _ {2} ]} \mid \left(a _ {1}, a _ {2}, \dots , a _ {n _ {1}}\right) \in C _ {1}, \left(b _ {1}, b _ {2}, \dots , b _ {n _ {2}}\right) \in C _ {2} \right\}
$$

An alternative definition is to place symbols from F in an $n _ { 1 } \times n _ { 2 }$ rectangular array such that each column is a codeword of $C _ { 1 }$ and each row is a codeword of $C _ { 2 }$ . See also [14].

Theorem 3. Let $C _ { 1 }$ be an $[ n _ { 1 } , k _ { 1 } , d _ { 1 } ]$ code and $C _ { 2 }$ be an $[ n _ { 2 } , k _ { 2 } , d _ { 2 } ]$ code. Then the code $C _ { 1 } \times C _ { 2 }$ is $a \; [ n _ { 1 } n _ { 2 } , k _ { 1 } k _ { 2 } , d _ { 1 } d _ { 2 } ]$ code with availability 2. Moreover, ${ i f }   C _ { 1 }$ has locality $r _ { 1 }$ and availability $l _ { 1 }$ and $C _ { 2 }$ has locality $r _ { 2 }$ and availability $l _ { 2 }$ , then $C _ { 1 } \times C _ { 2 }$ is a code of availability $l _ { 1 } + l _ { 2 }$ and locality $r _ { 1 } + r _ { 2 }$

Proof. Let D denote the minimum distance of the code $C _ { 1 } \times C _ { 2 }$ . If $( i , j )$ is a nonzero position, then there are $d _ { 1 }$ positions in the set $[ n _ { 1 } ] \times \{ j \}$ which have a nonzero entry. Suppose those nonzero positions are $( i _ { 1 } , j ) , ( i _ { 2 } , j ) , \ldots , ( i _ { d _ { 1 } } , j )$ For each of those nonzero positions $( i _ { s } , j )$ , there are $d _ { 2 }$ nonzero positions in $\{ i _ { s } \} \times [ n _ { 2 } ]$ . Thus there are at least $d _ { 1 } d _ { 2 }$ nonzero positions.

In order to prove equality, let $( a _ { 1 } , a _ { 2 } , \ldots , a _ { n _ { 1 } } )$ be a codeword of weight $d _ { 1 }$ in $C _ { 1 } .$ , and $( b _ { 1 } , b _ { 2 } , \ldots , b _ { n _ { 2 } } )$ be a codeword of weight $d _ { 2 }$ in $C _ { 2 }$ . Then the codeword defined by $c _ { i , j } = a _ { i } b _ { j }$ is the required codeword of weight $d _ { 1 } d _ { 2 }$

Let $I _ { 1 }$ be an information set for $C _ { 1 }$ and let $I _ { 2 }$ be an information set of $C _ { 2 }$ . The $i ^ { t h }$ coordinate of $c \in C _ { 1 }$ may be written as the linear combination $\mathbf { p _ { i } m _ { 1 } }$ for a message vector $m _ { 1 } \in \mathbb { F } ^ { I _ { 1 } }$ . Likewise, the $i ^ { t h }$ coordinate of $c \in C _ { 1 }$ may be written as the linear combination $\bf q _ { j } m _ { 2 }$ for a message vector $m _ { 2 }   \in   \mathbb { F } ^ { I _ { 2 } }$ . After placing any values in $I _ { 1 } \times I _ { 2 }$ , the remaining values are given by $c _ { i , j } = \mathbf { p _ { i } m _ { 1 } q _ { j } m _ { 2 } }$

Note that position $( i , j )$ is in the two sets $[ n _ { 1 } ] \times \{ j \}$ and $\{ i \} \times [ n _ { 2 } ]$ . These two sets have only $( i , j )$ in common. Thus, $[ n _ { 1 } ] \times \{ j \} \setminus ( i , j )$ and $\{ i \} \times [ n _ { 2 } ] \setminus ( i , j )$ are recovery sets for $( i , j ) ;$ note that they are disjoint as required for availability.

Consider $( i , j )   \in   [ n _ { 1 } ] \times [ n _ { 2 } ]$ . As $C _ { 1 }$ is a code of availability $l _ { 1 }$ there are $l _ { 1 }$ disjoint sets , $I _ { 1 } , I _ { , } 2 , \ldots , I _ { l _ { 1 } }$ in $[ n _ { 1 } ] \setminus \{ i \}$ from which position i may be recovered. Likewise as $C _ { 2 }$ is a code of availability $l _ { 2 }$ there are $l _ { 2 }$ disjoint sets , $, J _ { 1 } , J _ { 2 } , \ldots , J _ { l _ { 2 } }$ in $[ n _ { 2 } ] \backslash \{ j \}$ from which position j may be recovered. The sets $I _ { 1 } { \times } \{ j \} { , } I _ { 2 } { \times } \{ j \} , { \ldots } ,$ $I _ { l _ { 1 } } \times \{ j \} { , } \{ i \} \times J _ { 1 } { , } \{ i \} \times J _ { 2 } { , } \ldots { , } \{ i \} \times J _ { l _ { 2 } }$ are then $l _ { 1 } + l _ { 2 }$ recovery sets which are disjoint; this gives the desired availability.

## 4 Examples

A number of examples of LRCs are given in this section, and some comparisons are drawn between instances of the constructions discussed in this paper as well as those appearing elsewhere in the literature. In addition, we provide LRCs on the Ree curve via a construction that allows for computable parameters despite the issues mentioned in Remarks 2 and 3.

Tamo and Barg gave a seminal construction of an optimal LRC code of locality r in [21]. The LRC construction is based on a set $L \subseteq \mathbb { F } _ { q } ,$ , a partition of $L$ into disjoint subsets $A _ { 1 } ,   A _ { 2 } ,   . . . ,   A _ { m }$ where each set $A _ { i }$ has size $r + 1$ and a polynomial $g ( x )$ of degree $r + 1$ such that g is constant on each subset $A _ { i }$ . Tamo and Barg construct an LRC code from a subcode of the Reed–Solomon code over L of dimension $k ^ { \prime }$ by evaluating the functions of the form $X ^ { i } g ( X ) ^ { j }$ where $0 \leq i \leq r , i \neq s$ for a fixed $0 \leq s \leq r$ and $i + ( r + 1 ) j \leq k ^ { \prime } - 1$ . There are many partitions and many choices for $g ( X )$ . However, we shall focus on partitions given by linear subsets of $\mathbb { F } _ { q }$ or by cosets of the multiplicative group of $\mathbb { F } _ { q }$ . We shall use evaluation codes as a generalization of Reed–Solomon codes and AG codes.

Let $A = \{ \alpha _ { 1 } , \alpha _ { 2 } , \ldots , \alpha _ { n } \} \subseteq \mathbb { F } _ { q } ^ { m }$ . Let $f ( x _ { 1 } , x _ { 2 } , \ldots , x _ { m } )$ be a polynomial in m variables. The evaluation map of $f$ on A is defined as

$$
e v _ {A}: \mathbb {F} _ {q} [ x _ {1}, x _ {2}, \dots , x _ {m} ] \to \mathbb {F} _ {q} ^ {n}
$$

where

$$
e v _ {A} (f) = (f (\alpha_ {1}), f (\alpha_ {2}), \dots , f (\alpha_ {n})).
$$

We remark that the vanishing ideal of A, namely

$$
I _ {A} = \{f \in \mathbb {F} _ {q} [ x _ {1}, x _ {2}, \dots , x _ {m} ] | f (\alpha) = 0 \forall \alpha \in A \},
$$

is the kernel of the evaluation map $e v _ { A }$

Let $A { = } \{ \alpha _ { 1 } , \alpha _ { 2 } , \ldots , \alpha _ { n } \} { \subseteq } { \mathbb { F } } _ { q } ^ { m }$ . Let L be a subspace of $\mathbb { F } _ { q } [ x _ { 1 } , x _ { 2 } , \ldots , x _ { m } ]$ The set

$$
C (A, L) = \{e v _ {A} (f) \mid f \in L \}
$$

is known as an affine variety code. The definition of an affine variety code simply states that a linear code may be constructed by evaluating functions on a set of points. In most cases, the structure of L or A will imply certain properties of the code hold, such as dimension, minimum distance or locality.

Lemma 1. Let $V _ { 1 }   \subseteq   \mathbb { F } _ { q } ^ { m _ { 1 } }$ . Let $L _ { 1 }$ be a subspace of $\mathbb { F } _ { q } [ x _ { 1 } , x _ { 2 } , \ldots , x _ { m _ { 1 } } ]$ . Simi-$l a r l y ,$ take $V _ { 2 } \subseteq \mathbb { F } _ { q } ^ { m _ { 2 } }$ and $L _ { 2 }$ be a subspace ${ o f } \: \mathbb { F } _ { q } [ y _ { 1 } , y _ { 2 } , \ldots , y _ { m _ { 2 } } ]$ . Consider the evaluation codes: $\dot { C _ { 1 } } = C ( V _ { 1 } , L _ { 1 } )$ and $C _ { 2 } = C ( V _ { 2 } , L _ { 2 } )$ . The product code $C _ { 1 } \times C _ { 2 }$ is the evaluation code $C _ { 3 } = C ( V _ { 3 } , L _ { 3 } )$ , where $V _ { 3 } = \dot { V } _ { 1 } \times V _ { 2 } \subseteq \mathbb { F } _ { q } ^ { m _ { 1 } + m _ { 2 } }$ and the set of evaluated functions is $L _ { 3 } = \{ f ( X ) g ( Y ) , f \in L _ { 1 } , g \in L _ { 2 } \}$

Proof. It is clear that taking $f   \in   L _ { 1 }$ and $g   \in   L _ { 2 }$ and evaluating the product $f(X)g(Y)$ on the array $\{ ( \alpha , \beta )   |   \alpha   \in   V _ { 1 } , \beta   \in   V _ { 2 } \}$ will give a codeword of the form $(f(\alpha)g(\beta))$ . This codeword is also a codeword of $C _ { 1 } \times C _ { 2 }$

In order to prove equality, we use a dimensional analysis. $\mathrm { A s } C _ { 1 }$ is a code of dimension $k _ { 1 }$ , there exist $f _ { 1 } , f _ { 2 } , \ldots , f _ { k _ { 1 } }$ functions of $L _ { 1 }$ and $\alpha _ { 1 } , \alpha _ { 2 } , \ldots , \alpha _ { k _ { 1 } } \in V _ { 1 }$ such that $f _ { i } ( \alpha _ { j } ) = \delta _ { i , j }$ . Likewise, there exist $g _ { 1 } , g _ { 2 } , \ldots , g _ { k _ { 2 } }$ functions of $L _ { 2 }$ and $\beta _ { 1 } , \beta _ { 2 } , \ldots , \beta _ { k _ { 2 } } \in V _ { 2 }$ such that $g _ { i ^ { \prime } } ( \beta _ { j ^ { \prime } } )   =   \delta _ { i ^ { \prime } , j ^ { \prime } }$ . The evaluation of the functions $f _ { i } g _ { i ^ { \prime } }$ on the points $\alpha _ { j } \beta _ { j ^ { \prime } }$ will also imply the image has dimension $k _ { 1 } k _ { 2 }$

We will construct LRC codes based on the product code of the Tamo–Barg construction and Lemma 1. In particular, we shall take $V _ { 1 } \times V _ { 2 } , V _ { 1 } \times V _ { 2 } \times V _ { 3 }$ for $V _ { i } \subseteq \mathbb { F } _ { q }$ as our evaluation sets and the evaluation functions to be

$$
\left\{f _ {1} (X) f _ {2} (Y) f _ {3} (Z) \mid f _ {i} \in L _ {i} \right\}
$$

where $L _ { i } = \{ T ^ { a } g _ { i } ( T ) ^ { j } , 0 \leq a \leq r _ { i } , a \neq s _ { i } , a + ( r _ { i } + 1 ) j < k ^ { \prime } \}$

Note that classical Hermitian codes are obtained by evaluating monomials of the form $\mathcal { M } ( s )   : =   \{ X ^ { i } Y ^ { j } | i q + j ( q + 1 )   \leq   s \}$ on the $q ^ { 3 }$ points of the form $A = \{ ( \alpha , \beta ) \in \widehat { \mathbb { F } _ { q ^ { 2 } } ^ { 2 } } | \alpha ^ { \dot { q } + 1 } = \beta ^ { \dot { q } } + \beta , \alpha \neq 0 \}$ . However, as the vanishing ideal of A is the ideal spanned by $X ^ { q + 1 } - Y ^ { q } - Y$ and $X ^ { q ^ { 2 } - 1 } - 1$ , we may consider the Hermitian code obtained by evaluating the functions

$$
\mathcal {M} (s) = \{X ^ {i} Y ^ {j} \mid i q + j (q + 1) \leq s, 0 \leq i \leq q ^ {2} - 2, 0 \leq j \leq q - 1 \}.
$$

In order to find a subcode of the Hermitian code with a given locality, proceed as follows: Let $g _ { 1 } ( x )$ be a polynomial of degree $r _ { 1 }   +   1$ . Let $A _ { 1 } , A _ { 2 } , \ldots , A _ { \frac { q ^ { 2 } - 1 } { r _ { 1 } + 1 } }$ be a partition of $\mathbb { F } _ { q ^ { 2 } }$ into multiplicative cosets of $\mathbb { F } _ { q ^ { 2 } } ^ { * }$ where $g _ { 1 } ( X )$ is constant on each $A _ { i }$ . Likewise let $g _ { 2 } ( Y )$ be a polynomial of degree $r _ { 2 } + 1$ . Let $B _ { 1 } , B _ { 2 } , \ldots , B _ { \frac { q } { r _ { 2 } + 1 } }$ be a partitiion of $\{ \gamma | \gamma ^ { q } + \gamma = 0 \}$ where $g _ { 2 } ( Y )$ is constant on each $B _ { j }$ . For fixed $s _ { 1 } , s _ { 2 } ,$ , the code obtained by evaluating

$$
L _ {s 1, s 2} (s) = \left\{ \begin{array}{c c} & 0 \leq i _ {1} \leq r _ {1}, i _ {1} \neq s _ {1}, 0 \leq j _ {1} \leq r _ {2}, \\ X ^ {i _ {1}} g _ {1} (X) ^ {i _ {2}} Y ^ {j _ {1}} g _ {2} (Y) ^ {j _ {2}} \mid & 0 \leq i _ {1} + (r _ {1} + 1) i _ {2} \leq q ^ {2} - 2, \\ & 0 \leq j _ {1} + (r _ {2} + 1) j _ {2} \leq q - 1 \end{array} \right\}
$$

is a subcode of the Hermitian code of degree s which also has locality $r _ { 1 }$ and locality $r _ { 2 }$ with availability 2. In this case we obtain the codes over $\mathbb { F } _ { 1 6 }$ with locality

3 and availability 2 and the following parameters: [64, 1, 64], [64, 2, 60], [64, 3, 59], [64, 4, 56], [64, 5, 55], [64, 6, 54], [64, 7, 51], [64, 8, 50], [64, 9, 48], [64, 10, 46], [64, 11, 44], [64, 12, 43], [64, 13, 40], [64, 14, 39], [64, 15, 38], [64, 16, 35], [64, 17, 34], [64, 18, 32], [64, 19, 30], [64, 20, 28], [64, 21, 27], [64, 22, 24], [64, 23, 23], [64, 24, 22], [64, 25, 19], [64, 26, 18], [64, 27, 16], [64, 28, 14], [64, 29, 12], [64, 30, 12], [64, 31, 8], [64, 32, 8], [64, 33, 8], [64, 34, 6], [64, 35, 6], [64, 36, 4]. Note these codes handily outperform the product code construction from two Reed–Solomon code over $\mathbb { F } _ { 1 6 }$ , which have parameters: [64, 3, 56], [64, 6, 42], [64, 9, 28],[64, 12, 30], [64, 18, 20] [64, 27, 12].

For Suzuki curves, we use two different constructions of LRCs. As the full affine plane $\mathbb { F } _ { q } ^ { 2 }$ is the set of $\mathbb { F } _ { q ^ { - } }$ –rational points, we evaluate the intersection of a product code of two LRC codes with the Suzuki code of length $q ^ { 2 }$ . The vanishing ideal of the full affine plane is the ideal spanned by $X ^ { q } - X , Y ^ { q } - Y$ . In this case, the Suzuki code is obtained by taking the polynomials of low order at infinity and evaluating at the $q ^ { 2 }$ rational points. One can also take the remainders modulo $X ^ { q } { + } X , \bar { Y ^ { q } } { + } Y$ to determine the dimension of the code instead. Hence the Suzuki code is obtained by evaluating $L ( s ) =$

$$
\{X ^ {a} Y ^ {b} U ^ {c} V ^ {d} \mod X ^ {q} + X, Y ^ {q} + Y \mid a q + b (q + q _ {0}) + c (q + 2 q _ {0}) + d (q + 2 q _ {0} + 1) \leq s \}.
$$

In this case, the optimal LRC codes of locality 3 and length 8 have parameters: $[ 8 , 1 , 8 ] ,   \dot { [ 8 , 2 , 7 ] } , \ddot { [ } 8 , 3 , 6 ] ,   [ 8 , 4 , 4 ] ,   [ 8 , 5 , 3 ] ,   \dot { [ } 8 , \dot { 6 } , 2 ]$ . In order to get a product code of locality 3 and availability 2 from these codes, we get a [64, 10, 21] code. From the Suzuki code construction, after imposing additional LRC conditions, we get a [64, 10, 36] code with the same locality and availability parameters.

In the following table, we compare some Suzuki LRC codes with some RS product LRC codes. Both have the same length, symbols, locality and availability. Note that we were able to improve on most of the Product code constructions, except for [64, 25, 9]. We expect to improve our codes by improving the minimum distance bounds of the Suzuki codes.

To get LRCs from the Ree curve we shall make a similar construction to the codes from the Suzuki curve. Due to the abundance of possible codes and availabilities, we shall restrict ourselves to the case where $q = 2 7$ , availability is $3$ and $r _ { 1 } = r _ { 2 } = r _ { 3 } = 8$ . Please recall that the Ree curve $R _ { q }$ may be described by the equation

$$
R _ {q}: \left\{ \begin{array}{l} y ^ {q} - y = x ^ {q _ {0}} (x ^ {q} - x) \\ z ^ {q} - z = x ^ {2 q _ {0}} (x ^ {q} - x) \end{array} \right.
$$

where $q _ { 0 }   =   3 ^ { s } , \; q   =   3 q _ { 0 } ^ { 2 }$ , and $s   \in   \mathbb { N } .$ The valuation of $x$ at infinity is $q ,$ the valuation of $y$ at infinity is $q + q _ { 0 }$ and the valuation of z at infinity is $q +$ $2 q _ { 0 }$ . If G represents the pole at infinity of the Ree curve, and D is the divisor corresponding to the $\mathbf { F } _ { 2 7 }$ –affine points of the Ree curve, then the code $C ( s G , D )$ is the algebraic geometry code obtained by evaluating all functions having poles only at infinity of order $\leq s$ . We shall compare LRC subcodes of $C ( s G , D )$ with product codes of Reed–Solomon codes. We shall use a particular Tamo–Barg construction [21] for $\mathbf { F } _ { 2 7 }$ . In this case, our sets will be places where the trace is

| [s1,s2] | Suzuki code | Suzuki code with locality and availability |
| --- | --- | --- |
| [1,0] | [64,1,64] | [64,1,64] |
| [1,0] | [64,2,56] | [64,2,56] |
| [2,0] | [64,3,54] | [64,3,54] |
| [3,0] | [64,5,51] | [64,4,51] |
| [3,0] | [64,6,48] | [64,5,48] |
| [3,0] | [64,7,46] | [64,6,46] |
| [3,0] | [64,8,44] | [64,7,44] |
| [3,0] | [64,12,40] | [64,8,40] |
| [3,0] | [64,14,38] | [64,9,38] |
| [3,0] | [64,15,36] | [64,10,36] |
| [2,0] | [64,19,32] | [64,11,32] |
| [3,0] | [64,20,31] | [64,12,31] |
| [3,0] | [64,21,30] | [64,13,30] |
| [3,0] | [64,23,28] | [64,14,28] |
| [3,0] | [64,24,27] | [64,15,27] |
| [3,0] | [64,25,26] | [64,16,26] |
| [3,0] | [64,27,24] | [64,17,24] |
| [3,0] | [64,29,22] | [64,18,22] |
| [3,0] | [64,31,20] | [64,19,20] |
| [3,0] | [64,32,19] | [64,20,19] |
| [3,0] | [64,35,16] | [64,21,16] |
| [3,0] | [64,37,14] | [64,22,14] |
| [3,0] | [64,39,12] | [64,23,12] |
| [3,0] | [64,40,11] | [64,24,11] |
| [3,0] | [64,43,8] | [64,25,8] |
| [3,0] | [64,44,7] | [64,26,7] |
| [3,0] | [64,45,6] | [64,27,6] |
| [3,0] | [64,47,4] | [64,28,4] |
| [3,0] | [64,49,2] | [64,29,2] |

Table 1. Comparison of parameters of codes from the Suzuki curve using a standard AG code construction and those with locality and availability

| Suzuki code with locality and availability | Comparable Product code |
| --- | --- |
| [64,1,64] | [64,1,64] |
| [64,2,56] | [64,2,56] |
| [64,3,54] | [64,3,48] |
| [64,4,51] | [64,4,49] |
| [64,5,48] | [64,5,24] |
| [64,6,46] | [64,6,42] |
| [64,7,44] | [64,6,42] |
| [64,8,40] | [64,8,28] |
| [64,9,38] | [64,9,36] |
| [64,10,36] | [64,10,21] |
| [64,11,32] | [64,10,21] |
| [64,12,31] | [64,12,24] |
| [64,13,30] | [64,12,24] |
| [64,14,28] | [64,12,24] |
| [64,15,27] | [64,15,18] |
| [64,16,26] | [64,16,16] |
| [64,17,24] | [64,16,16] |
| [64,18,22] | [64,18,12] |
| [64,19,20] | [64,18,12] |
| [64,20,19] | [64,20,12] |
| [64,21,16] | [64,20,12] |
| [64,22,14] | [64,20,12] |
| [64,23,12] | [64,20,12] |
| [64,24,11] | [64,24,8] |
| [64,25,8] | [64,25,9] |
| [64,26,7] | [64,25,9] |
| [64,27,6] | [64,25,9] |
| [64,28,4] | [64,25,9] |
| [64,29,2] | [64,25,9] |

Table 2. Comparison of parameters of codes from the Suzuki curve with locality and availability and product codes

constant $A_{0} = \left\{ a \in \mathbf{F}_{27} \mid a + a^{3} + a^{9} = 0 \right\}, A_{1} = \left\{ a \in \mathbf{F}_{27} \mid a + a^{3} + a^{9} = 1 \right\}$ and $A_{2} = \{ a \in \mathbf{F}_{27} | a + a^{3} + a^{9} = 2 \}$ . The polynomial $L ( T ) = T + T ^ { 3 } + T ^ { 9 }$ is constant on the recovery sets $A _ { 0 } , A _ { 1 } , A _ { 2 }$ and the evaluation of the polynomials in $\{ L ( T ) ^ { i } T ^ { j } \mid 0 \leq i \leq 2 , 0 \leq j \leq 7 , 2 i + j \leq k \}$ gives a subcode of the Reed–Solomon code $R S _ { 2 7 } ( \mathbf { F } _ { 2 7 } , k )$ which is an LRC with locality 8.

The possible Reed–Solomon codes with locality 8 and length 27 of this form are: [27, 1, 27], [27, 2, 26], [27, 3, 25], [27, 4, 24], [27, 5, 23], [27, 6, 22], [27, 7, 21], [27, 8, 20], [27, 9, 18], [27, 10, 17], [27, 11, 16], [27, 12, 15], [27, 13, 14], [27, 14, 13], [27, 15, 12], [27, 16, 11], [27, 17, 9], [27, 18, 8], [27, 19, 7], [27, 20, 6], [27, 21, 5], [27, 22, 4], [27, 23, 3], [27, 24, 2].

The key idea of this proof is that both the product code of Reed–Solomon codes and the subcodes of the Ree AG code may be considered as evaluation codes of combinations of monomials in $\mathcal { L } = \{ X ^ { i } Y ^ { j } Z ^ { l } | 0 \leq i , j , l \leq 2 6 \}$ . In the $\mathbf { F } _ { 2 7 } ^ { 3 }$

The subcode of $C ( s G , D )$ is obtained by evaluating the monomials in

$$
\mathcal {L} (s) = \{X ^ {a} Y ^ {b} Z ^ {c} \in \mathcal {L} \mid a q + b (q + q _ {0}) + c (q + 2 q _ {0}) \leq s, 0 \leq a, b, c \leq q - 1 \}.
$$

The subcode of $C ( s G , D )$ with locality 8 and availability 3 is given by evaluating polynomials of the form $\left[ L ( X ) ^ { a 1 } X ^ { a _ { 2 } } \mathring { L } ( Y ) ^ { b 1 } Y ^ { b 2 } L ( Z ) ^ { c 1 } \mathring { Z } ^ { c 2 } \right.$ where $0 \leq a _ { 1 } , b _ { 1 } , c _ { 1 } \leq$ $2 , 0 \leq a _ { 2 } , b _ { 2 } , c _ { 2 } \leq 7$ and subject to the degree constrain that the polynomials should also be in $\mathcal { L } ( s )$ . Note that depending on the parameters of the codes we might find better Reed–Solomon product codes as LRCs or better LRC subcodes from the Ree curve.

For example, comparing codes with minimum distance 600 we get an LRC with parameters [19683, 4536, 600], locality 8 and availability 3 from the product code construction of the Reed–Solomon code and a [19683, 2937, 600] LRC with locality 8 and availability 3 from $C ( s G , D )$ However, comparing codes with dimension 200 we get a [19683, 200, 10580] LRC with locality 8 and availability 3 from the product code construction of the Reed–Solomon code and a [19683, 201, 13086] LRC with locality 8 and availability 3 from the AG code.

There is also an LRC construction using the codes $C ( s G , D )$ . In this case note that for the same replication sets $A _ { 0 } ,   A _ { 1 }$ and $A _ { 2 } ,$ the dual code of an LRC with locality 8 is generated by evaluating $\{ L ( T ) ^ { i } | 0 \leq i \leq 2 \}$ . If we extend this to $\mathbf { F } _ { 2 7 } ^ { 3 }$ we can get a code with locality 8 and availability 3 as the dual code of the evaluation of $\{ L ( X ) ^ { i } L ( Y ) ^ { j } L ( Z ) ^ { k } \mid 0 \leq i \leq 2 \}$ . In order to find subcodes of $C ( s G , D )$ with locality 8 and availability 3 we consider how many of the functions $\{ \widetilde{L(X)^i} \widetilde{L(Y)^j} L(Z)^k | 0 \leq i \leq 2 \}$ also have weight s. The dimension of the LRC is found by computing the dimension of ${ \mathcal { L } } ( s )   +   \{ L ( X ) ^ { i } L ( Y ) ^ { j } L ( Z ) ^ { k } \mid 0 \leq i \leq 2 \}$

In this case we have found a [19683, 4536, 600] LRC with locality 8 and availability 3 from the product code construction of the Reed–Solomon code and a [19683, 15434, 600] LRC with locality 8 and availability 3 from $C ( s G , D ) ^ { \perp }$

We have found instances in which the Reed–Solomon product codes are better than the LRC from the AG codes. Likewise, we have found cases in which the AG LRCs outperform the Reed–Solomon codes. Further improvements could be possible as knowledge of the Riemann-Roch spaces of the Ree curve improves.

## References

1. Barg, A., Haymaker, K., Howe, E., Matthews, G. L., Varilly-Alvarado A.: Locally recoverable codes from algebraic curves and surfaces, in Algebraic Geometry for Coding Theory and Cryptography, E.W. Howe, K.E. Lauter, and J.L. Walker, Editors, Springer, 2017, pp. 95–126. doi: 10.1007/978-3-319-63931-4 4

2. Ballentine, S., Barg, A., Vlăduţ, S.: Codes with hierarchical locality from covering maps of curves, IEEE Transactions on Information Theory, vol. 65, no. 10, pp. 6056–6071, Oct. 2019. doi: 10.1109/TIT.2019.2919830

3. Eid, A., Duursma, I.: Smooth embeddings for the Suzuki and Ree curves. Algorithmic arithmetic, geometry, and coding theory, vol. 637, 251-291, 2015. doi: 10.1090/conm/637/12763

4. Eid, A., Hasson, H., Ksir, A. , Peachey, J.: Suzuki-invariant codes from the Suzuki curve. Designs, Codes and Cryptography, vol. 81, pp. 413– 425, 2016. doi: 10.1007/s10623-015-0164-5

5. Giulietti, M., Korchmros, G.: A new family of maximal curves over a finite field, Mathematische Annalen, vol. 343, article 229, 2009. doi: 10.1007/s00208-008-0270- z

6. Giulietti, M., Montanucci, M., Quoos, L., Zini, G.: On some Galois covers of the Suzuki and Ree curves, Journal of Number Theory, vol. 189, pp. 220–254. doi: 10.1016/j.jnt.2017.12.005.

7. Guruswami, V., Jin, L., Xing, C.: Constructions of maximally recoverable local reconstruction codes via function fields, International Colloquium on Automata, Languages, and Programming, 2019. doi: 10.4230/LIPIcs.ICALP.2019.68

8. Hansen, J.P., Stichtenoth, H.: Group codes on certain algebraic curves with many rational points, Applicable Algebra in Engineering, Communication and Computing, vol. 1, pp. 67–77, 1990. doi: 10.1007/BF01810849

9. Haymaker, K., Malmskog, B., Matthews, G. L.: Locally recoverable codes with availability t ≥ 2 from fiber products of curves, Advances in Mathematics of Communications, vol. 12 (2), pp. 317–336, 2018. doi: 10.3934/amc.2018020

10. Hirschfeld, J., Korchmáros, G., Torres, F, Algebraic Curves over a Finite Field, PRINCETON; OXFORD: Princeton University Press, 2008. doi:10.2307/j.ctt1287kdw

11. Jin, L., Ma, L., Xing, C.: Construction of optimal locally repairable codes via automorphism groups of rational function fields, IEEE Transactions on Information Theory, vol. 66, no. 1, pp. 210–221, Jan. 2020. doi: 10.1109/TIT.2019.2946637

12. Kirfel, C., Pellikaan, R.: The minimum distance of codes in an array coming from telescopic semigroups, IEEE Transactions on Information Theory, vol. 41, no. 6, pp. 1720–1732, Nov. 1995. doi: 10.1109/18.476245

13. Li, X., Ma, L., and Xing, C.: Optimal locally repairable codes via elliptic curves, IEEE Transactions on Information Theory, vol. 65, no. 1, pp. 108–117, Jan. 2019. doi: 10.1109/TIT.2018.2844216

14. López, H., Matthews, G. L., Soprunov, I.: Monomial-Cartesian codes and their duals, with applications to LCD codes, quantum codes, and locally recoverable codes, Designs, Codes and Cryptography, 2020. doi:10.1007/s10623-020-00726-x

15. Matthews, G. L.: Codes from the Suzuki function field. IEEE Transactions on Information Theory, vol. 50, no. 12, pp. 3298–3302, Dec. 2004. doi: 10.1109/TIT.2004.838102.

16. Montanucci, M., Timpanella, M., Zini, G.: AG codes and AG quantum codes from cyclic extensions of the Suzuki and Ree curves, Journal of Geometry, vol. 109, article 23, 2018. doi: 10.1007/s00022-018-0428-0.

17. Pedersen, J. P.: A function field related to the Ree group, Coding theory and algebraic geometry (Luminy, 1991), Lecture Notes in Math., vol. 1518, Springer, Berlin, 1992, pp. 122–31. doi: 10.1007/BFb0087997

18. Skabelund, D. C.: New maximal curves as ray class fields over Deligne-Lusztig curves. Proceedings of the American Mathematical Society, vol. 146, no. 2, pp. 525–540. doi: 10.1090/proc/13753

19. Tafazolian, S., Torres, F.: On the Ree curve, Journal of Pure and Applied Algebra, vol. 223, no. 9, pp. 3831–3842, 2019. doi: 10.1016/j.jpaa.2018.12.006

20. Tamo, I., Barg, A.: Bounds on locally recoverable codes with multiple recovering sets, 2014 IEEE International Symposium on Information Theory, Honolulu, HI, 2014, pp. 691-695. doi: 10.1109/ISIT.2014.6874921

21. Tamo, I., Barg, A.: A Family of Optimal Locally Recoverable Codes, IEEE Transactions on Information Theory, vol. 60, no. 8, pp. 4661–4676, Aug. 2014. doi: 10.1109/TIT.2014.2321280

22. Barg, A., Tamo, I., Vlăduţ: Locally Recoverable Codes on Algebraic Curves, IEEE Transactions on Information Theory, vol. 63, no. 8, pp. 4928-4939, Aug. 2017. doi: 10.1109/TIT.2017.2700859

23. Tsfasman, M., Vlăduţ, S., Zink, T.: Modular curves, Shimura curves and Goppa codes better than Varshamov-Gilbert bound, Mathematische Nachrichten, vol. 109, pp. 21–28, 1982. doi: 10.1002/mana.19821090103