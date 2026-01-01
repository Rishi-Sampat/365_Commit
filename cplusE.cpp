#include <bits/stdc++.h>
using namespace std;

/*
  ReduceExp - robust solver
  - Parse expression (numbers, x, y, +, *, parentheses)
  - Convert to polynomial coefficients for monomials:
      coeff[x^2], coeff[xy], coeff[y^2], coeff[x], coeff[y], coeff[1]
  - Compute minimal eval cost (multiplications + additions) by building
    an AST for the polynomial, simplifying (constant fold, remove *1, +0)
    and then counting (sum over ADD nodes: children-1) + (sum over MUL nodes: children-1)
  - Try all valid factorizations of Forms 1..3 (with variable swaps)
    by enumerating factor pairs of the relevant coefficients within [-100,100].
  - For each valid factor candidate, build the AST for (factor) + leftover,
    simplify and count ops. Output the minimal ops across "no-factor" and candidates.
*/

struct Poly {
    // coeff[i][j] for x^i * y^j  where i,j in {0,1,2}
    int c[3][3];
    Poly(){ memset(c,0,sizeof(c)); }
};

string s;
int posn;

// forward declarations
Poly parseExpr();
Poly parseTerm();
Poly parseFactor();

void skipSpaces(){
    while(posn < (int)s.size() && isspace((unsigned char)s[posn])) posn++;
}

long long parseInt() {
    skipSpaces();
    long long sign = 1;
    if (posn < (int)s.size() && s[posn] == '+') { posn++; }
    // no explicit '-' in problem statement but support if present
    if (posn < (int)s.size() && s[posn] == '-') { sign = -1; posn++; }
    skipSpaces();
    long long val = 0;
    bool any = false;
    while (posn < (int)s.size() && isdigit((unsigned char)s[posn])) {
        any = true;
        val = val*10 + (s[posn]-'0');
        posn++;
    }
    if(!any) return 0;
    return sign * val;
}

Poly addPoly(const Poly &A, const Poly &B){
    Poly R;
    for(int i=0;i<=2;i++) for(int j=0;j<=2;j++) R.c[i][j] = A.c[i][j] + B.c[i][j];
    return R;
}

Poly mulPoly(const Poly &A, const Poly &B){
    Poly R;
    for(int i1=0;i1<=2;i1++) for(int j1=0;j1<=2;j1++){
        if(A.c[i1][j1]==0) continue;
        for(int i2=0;i2<=2;i2++) for(int j2=0;j2<=2;j2++){
            if(B.c[i2][j2]==0) continue;
            int ni = i1 + i2;
            int nj = j1 + j2;
            if(ni <= 2 && nj <= 2){
                R.c[ni][nj] += A.c[i1][j1] * B.c[i2][j2];
            } else {
                // per problem there will be no cubic/higher; ignore otherwise
            }
        }
    }
    return R;
}

Poly parseExpr(){
    skipSpaces();
    Poly res = parseTerm();
    skipSpaces();
    while(posn < (int)s.size()){
        skipSpaces();
        if(posn < (int)s.size() && s[posn] == '+'){
            posn++;
            Poly t = parseTerm();
            res = addPoly(res, t);
        } else break;
    }
    return res;
}

Poly parseTerm(){
    skipSpaces();
    Poly res = parseFactor();
    skipSpaces();
    while(posn < (int)s.size()){
        skipSpaces();
        if(posn < (int)s.size() && s[posn] == '*'){
            posn++;
            Poly t = parseFactor();
            res = mulPoly(res, t);
        } else break;
    }
    return res;
}

Poly parseFactor(){
    skipSpaces();
    if(posn >= (int)s.size()) return Poly();
    if(s[posn] == '('){
        posn++;
        Poly inner = parseExpr();
        skipSpaces();
        if(posn < (int)s.size() && s[posn] == ')') posn++;
        return inner;
    } else if(isdigit((unsigned char)s[posn]) || s[posn]=='+' || s[posn]=='-'){
        long long v = parseInt();
        Poly p; 
        p.c[0][0] = (int)v;
        return p;
    } else if(s[posn] == 'x' || s[posn] == 'y'){
        char ch = s[posn++];
        Poly p;
        if(ch=='x') p.c[1][0] = 1;
        else p.c[0][1] = 1;
        return p;
    } else {
        // unknown token; skip
        posn++;
        return Poly();
    }
}

// AST nodes for simplified counting
enum NodeType { N_CONST, N_VAR, N_ADD, N_MUL };
struct Node {
    NodeType t;
    long long val;            // for CONST
    char var;                 // 'x' or 'y' for VAR
    vector<shared_ptr<Node>> ch;
    Node(long long v): t(N_CONST), val(v), var(0) {}
    Node(char c): t(N_VAR), val(0), var(c) {}
    Node(NodeType tp): t(tp), val(0), var(0) {}
};

// Build AST from polynomial
shared_ptr<Node> buildMonomialNode(int coeff, int xi, int yi){
    // coeff * x^xi * y^yi
    if(coeff == 0) return make_shared<Node>(0LL);
    if(xi==0 && yi==0){
        return make_shared<Node>((long long)coeff);
    }
    vector<shared_ptr<Node>> parts;
    if(coeff != 1){
        parts.push_back(make_shared<Node>((long long)coeff));
    }
    for(int i=0;i<xi;i++) parts.push_back(make_shared<Node>('x'));
    for(int j=0;j<yi;j++) parts.push_back(make_shared<Node>('y'));
    if(parts.size() == 1) return parts[0];
    auto m = make_shared<Node>(N_MUL);
    m->ch = parts;
    return m;
}

shared_ptr<Node> buildASTFromPoly(const Poly &P){
    vector<shared_ptr<Node>> terms;
    for(int i=0;i<=2;i++) for(int j=0;j<=2;j++){
        int coeff = P.c[i][j];
        if(coeff != 0){
            terms.push_back(buildMonomialNode(coeff, i, j));
        }
    }
    if(terms.empty()) return make_shared<Node>(0LL);
    if(terms.size() == 1) return terms[0];
    auto a = make_shared<Node>(N_ADD);
    a->ch = terms;
    return a;
}

// Simplify AST: flatten ADD/MUL, constant-fold, remove *1 and +0
shared_ptr<Node> simplify(shared_ptr<Node> n){
    if(n->t == N_CONST || n->t == N_VAR) return n;
    if(n->t == N_ADD){
        vector<shared_ptr<Node>> newch;
        long long constSum = 0;
        for(auto &c : n->ch){
            auto sc = simplify(c);
            if(sc->t == N_ADD){
                for(auto &g : sc->ch) newch.push_back(g);
            } else if(sc->t == N_CONST){
                constSum += sc->val;
            } else {
                newch.push_back(sc);
            }
        }
        if(constSum != 0) newch.push_back(make_shared<Node>(constSum));
        // remove any zero-const children
        vector<shared_ptr<Node>> finalch;
        for(auto &ch : newch){
            if(ch->t == N_CONST && ch->val == 0) continue;
            finalch.push_back(ch);
        }
        if(finalch.empty()) return make_shared<Node>(0LL);
        if(finalch.size() == 1) return finalch[0];
        auto out = make_shared<Node>(N_ADD);
        out->ch = finalch;
        return out;
    } else { // MUL
        vector<shared_ptr<Node>> newch;
        long long constProd = 1;
        bool hasConst = false;
        for(auto &c: n->ch){
            auto sc = simplify(c);
            if(sc->t == N_MUL){
                for(auto &g : sc->ch) newch.push_back(g);
            } else if(sc->t == N_CONST){
                constProd *= sc->val;
                hasConst = true;
            } else {
                newch.push_back(sc);
            }
        }
        // If any constant becomes zero -> whole product zero
        if(hasConst && constProd == 0) return make_shared<Node>(0LL);
        // remove multiplicative identity 1
        vector<shared_ptr<Node>> finalch;
        if(!(hasConst && constProd == 1)){
            if(hasConst) finalch.push_back(make_shared<Node>(constProd));
        }
        for(auto &ch : newch) finalch.push_back(ch);
        // if no children (only const 1 case)
        if(finalch.empty()) return make_shared<Node>(1LL);
        if(finalch.size() == 1) return finalch[0];
        auto out = make_shared<Node>(N_MUL);
        out->ch = finalch;
        return out;
    }
}

long long countOps(shared_ptr<Node> n){
    if(n->t == N_CONST || n->t == N_VAR) return 0;
    if(n->t == N_ADD){
        long long sum = 0;
        for(auto &c : n->ch) sum += countOps(c);
        long long adds = (long long)max(0, (int)n->ch.size() - 1);
        return sum + adds;
    } else {
        long long sum = 0;
        for(auto &c : n->ch) sum += countOps(c);
        long long mults = (long long)max(0, (int)n->ch.size() - 1);
        return sum + mults;
    }
}

// Factor pairs within bound [-100,100]
vector<pair<int,int>> factorPairs(int v){
    vector<pair<int,int>> res;
    const int LIM = 100;
    if(v == 0){
        // all pairs (0,b) and (a,0) within bounds
        for(int a=-LIM;a<=LIM;a++){
            if(abs(a) <= LIM){
                // (a,0)
                if(a*0 == v) res.emplace_back(a, 0);
            }
        }
        for(int b=-LIM;b<=LIM;b++){
            if(abs(b) <= LIM){
                if(0*b == v) res.emplace_back(0, b);
            }
        }
        // remove duplicates
        sort(res.begin(), res.end());
        res.erase(unique(res.begin(), res.end()), res.end());
        return res;
    } else {
        for(int a=-LIM; a<=LIM; a++){
            if(a == 0) continue;
            if(v % a == 0){
                int b = v / a;
                if(abs(b) <= LIM) res.emplace_back(a,b);
            }
        }
        // include negative divisors etc handled above
        // ensure unique
        sort(res.begin(), res.end());
        res.erase(unique(res.begin(), res.end()), res.end());
        return res;
    }
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    if(!std::getline(cin, s)) return 0;
    posn = 0;
    Poly P = parseExpr();

    // Build AST for original polynomial and compute ops (no factoring)
    auto baseAST = buildASTFromPoly(P);
    auto baseS = simplify(baseAST);
    long long baseOps = countOps(baseS);
    long long bestOps = baseOps;

    // helper to get coefficient shortcuts
    auto getc = [&](int xi, int yi)->int{ return P.c[xi][yi]; };

    // We'll try Forms:
    // Form1: (a1*v + b1)*(a2*v + b2), v is either x or y
    // Form2: (a1*x + b1)*(a2*y + b2)
    // Form3: (a1*x + b1*y)*(a2*x + b2) and also its variable-swapped variants

    const int LIM = 100;

    // --- Form1 for v = x or y
    for(int which = 0; which < 2; ++which){
        // which==0 => v = x ; which==1 => v = y
        int c_v2 = (which==0 ? P.c[2][0] : P.c[0][2]);
        int c_v1 = (which==0 ? P.c[1][0] : P.c[0][1]);
        int c_0  = P.c[0][0];
        auto aPairs = factorPairs(c_v2);
        auto bPairs = factorPairs(c_0);
        for(auto ap : aPairs){
            int a1 = ap.first, a2 = ap.second;
            for(auto bp : bPairs){
                int b1 = bp.first, b2 = bp.second;
                if(a1 * b2 + a2 * b1 != c_v1) continue;
                // construct factor AST and leftover polynomial
                Poly factPart; // initialize zero
                if(which==0){ // x
                    factPart.c[2][0] = a1*a2;
                    factPart.c[1][0] = a1*b2 + a2*b1;
                    factPart.c[0][0] = b1*b2;
                } else { // y
                    factPart.c[0][2] = a1*a2;
                    factPart.c[0][1] = a1*b2 + a2*b1;
                    factPart.c[0][0] = b1*b2;
                }
                // leftover = P - factPart
                Poly leftover;
                for(int i=0;i<=2;i++) for(int j=0;j<=2;j++) leftover.c[i][j] = P.c[i][j] - factPart.c[i][j];

                // Build factor AST: (a1*v + b1) * (a2*v + b2)
                auto left = make_shared<Node>(N_ADD);
                left->ch.clear();
                // left children: a1*v (if a1!=0) and b1 (if b1!=0)
                if(a1 != 0){
                    if(a1 == 1){
                        left->ch.push_back(make_shared<Node>( which==0 ? 'x' : 'y' ));
                    } else {
                        auto m = make_shared<Node>(N_MUL);
                        m->ch.push_back(make_shared<Node>((long long)a1));
                        m->ch.push_back( make_shared<Node>( which==0 ? 'x' : 'y' ) );
                        left->ch.push_back(m);
                    }
                }
                if(b1 != 0) left->ch.push_back(make_shared<Node>((long long)b1));
                if(left->ch.empty()) left = make_shared<Node>((long long)0);

                auto right = make_shared<Node>(N_ADD);
                right->ch.clear();
                if(a2 != 0){
                    if(a2 == 1){
                        right->ch.push_back(make_shared<Node>( which==0 ? 'x' : 'y' ));
                    } else {
                        auto m = make_shared<Node>(N_MUL);
                        m->ch.push_back(make_shared<Node>((long long)a2));
                        m->ch.push_back( make_shared<Node>( which==0 ? 'x' : 'y' ) );
                        right->ch.push_back(m);
                    }
                }
                if(b2 != 0) right->ch.push_back(make_shared<Node>((long long)b2));
                if(right->ch.empty()) right = make_shared<Node>((long long)0);

                auto factorNode = make_shared<Node>(N_MUL);
                factorNode->ch = { left, right };

                auto leftoverNode = buildASTFromPoly(leftover);
                auto top = make_shared<Node>(N_ADD);
                top->ch = { factorNode, leftoverNode };
                auto simp = simplify(top);
                long long ops = countOps(simp);
                bestOps = min(bestOps, ops);
            }
        }
    }

    // --- Form2: (a1*x + b1)*(a2*y + b2)
    {
        int c_xy = P.c[1][1];
        int c_x = P.c[1][0];
        int c_y = P.c[0][1];
        int c_0 = P.c[0][0];
        auto aPairs = factorPairs(c_xy);
        auto bPairs = factorPairs(c_0);
        for(auto ap : aPairs){
            int a1 = ap.first, a2 = ap.second;
            for(auto bp : bPairs){
                int b1 = bp.first, b2 = bp.second;
                if(a1 * b2 != c_x) continue;
                if(a2 * b1 != c_y) continue;
                // expansion check: a1*a2 xy ; a1*b2 x ; a2*b1 y ; b1*b2 const
                Poly factPart;
                factPart.c[1][1] = a1*a2;
                factPart.c[1][0] = a1*b2;
                factPart.c[0][1] = a2*b1;
                factPart.c[0][0] = b1*b2;
                Poly leftover;
                for(int i=0;i<=2;i++) for(int j=0;j<=2;j++) leftover.c[i][j] = P.c[i][j] - factPart.c[i][j];
                // build AST
                // left = (a1*x + b1)
                auto left = make_shared<Node>(N_ADD); left->ch.clear();
                if(a1 != 0){
                    if(a1==1) left->ch.push_back(make_shared<Node>('x'));
                    else {
                        auto m = make_shared<Node>(N_MUL);
                        m->ch.push_back(make_shared<Node>((long long)a1));
                        m->ch.push_back(make_shared<Node>('x'));
                        left->ch.push_back(m);
                    }
                }
                if(b1 != 0) left->ch.push_back(make_shared<Node>((long long)b1));
                if(left->ch.empty()) left = make_shared<Node>((long long)0);

                // right = (a2*y + b2)
                auto right = make_shared<Node>(N_ADD); right->ch.clear();
                if(a2 != 0){
                    if(a2==1) right->ch.push_back(make_shared<Node>('y'));
                    else {
                        auto m = make_shared<Node>(N_MUL);
                        m->ch.push_back(make_shared<Node>((long long)a2));
                        m->ch.push_back(make_shared<Node>('y'));
                        right->ch.push_back(m);
                    }
                }
                if(b2 != 0) right->ch.push_back(make_shared<Node>((long long)b2));
                if(right->ch.empty()) right = make_shared<Node>((long long)0);

                auto factorNode = make_shared<Node>(N_MUL);
                factorNode->ch = { left, right };
                auto leftoverNode = buildASTFromPoly(leftover);
                auto top = make_shared<Node>(N_ADD);
                top->ch = { factorNode, leftoverNode };
                auto simp = simplify(top);
                long long ops = countOps(simp);
                bestOps = min(bestOps, ops);
            }
        }
    }

    // --- Form3: (a1*x + b1*y)*(a2*x + b2)
    // also check swapped variables versions equivalent
    // We'll attempt both variants: original and swap x<->y (which is another valid form variant)
    for(int variant = 0; variant < 2; ++variant){
        // variant 0: as given (a1*x + b1*y)*(a2*x + b2)
        // variant 1: swap x<->y (so it's (a1*y + b1*x)*(a2*y + b2))
        int c_x2 = (variant==0 ? P.c[2][0] : P.c[0][2]); // a1*a2
        int c_xy = (variant==0 ? P.c[1][1] : P.c[1][1]); // a2*b1
        int c_x = (variant==0 ? P.c[1][0] : P.c[0][1]);  // a1*b2   (x linear in variant0, else y linear)
        int c_y = (variant==0 ? P.c[0][1] : P.c[1][0]);  // b1*b2   (y linear or x linear)
        // But to map correct equations we will directly use expansion formula:
        // variant==0:
        //   c_x2 = a1*a2
        //   c_xy = a2*b1
        //   c_x = a1*b2
        //   c_y = b1*b2
        // variant==1 swap x<->y:
        //   expansion of (a1*y + b1*x)*(a2*y + b2) gives:
        //   c_y2 = a1*a2
        //   c_xy = a2*b1
        //   c_y = a1*b2
        //   c_x = b1*b2
        // We'll implement explicit mapping below.
        if(variant == 0){
            int A = P.c[2][0]; // c_x2
            int B = P.c[1][1]; // c_xy
            int C = P.c[1][0]; // c_x
            int D = P.c[0][1]; // c_y
            auto aPairs = factorPairs(A);
            auto bPairs = factorPairs(D); // since b1*b2 = c_y (D)
            for(auto ap : aPairs){
                int a1 = ap.first, a2 = ap.second;
                for(auto bp : bPairs){
                    int b1 = bp.first, b2 = bp.second;
                    if(a2 * b1 != B) continue;
                    if(a1 * b2 != C) continue;
                    // expansion good
                    Poly factPart;
                    factPart.c[2][0] = a1*a2;
                    factPart.c[1][1] = a2*b1;
                    factPart.c[1][0] = a1*b2;
                    factPart.c[0][1] = b1*b2;
                    Poly leftover;
                    for(int i=0;i<=2;i++) for(int j=0;j<=2;j++) leftover.c[i][j] = P.c[i][j] - factPart.c[i][j];
                    // Build AST
                    // left = a1*x + b1*y
                    auto left = make_shared<Node>(N_ADD); left->ch.clear();
                    if(a1 != 0){
                        if(a1==1) left->ch.push_back(make_shared<Node>('x'));
                        else {
                            auto m = make_shared<Node>(N_MUL);
                            m->ch.push_back(make_shared<Node>((long long)a1));
                            m->ch.push_back(make_shared<Node>('x'));
                            left->ch.push_back(m);
                        }
                    }
                    if(b1 != 0){
                        if(b1==1) left->ch.push_back(make_shared<Node>('y'));
                        else {
                            auto m = make_shared<Node>(N_MUL);
                            m->ch.push_back(make_shared<Node>((long long)b1));
                            m->ch.push_back(make_shared<Node>('y'));
                            left->ch.push_back(m);
                        }
                    }
                    if(left->ch.empty()) left = make_shared<Node>((long long)0);
                    // right = a2*x + b2
                    auto right = make_shared<Node>(N_ADD); right->ch.clear();
                    if(a2 != 0){
                        if(a2==1) right->ch.push_back(make_shared<Node>('x'));
                        else {
                            auto m = make_shared<Node>(N_MUL);
                            m->ch.push_back(make_shared<Node>((long long)a2));
                            m->ch.push_back(make_shared<Node>('x'));
                            right->ch.push_back(m);
                        }
                    }
                    if(b2 != 0) right->ch.push_back(make_shared<Node>((long long)b2));
                    if(right->ch.empty()) right = make_shared<Node>((long long)0);
                    auto factorNode = make_shared<Node>(N_MUL);
                    factorNode->ch = { left, right };
                    auto leftoverNode = buildASTFromPoly(leftover);
                    auto top = make_shared<Node>(N_ADD);
                    top->ch = { factorNode, leftoverNode };
                    auto simp = simplify(top);
                    long long ops = countOps(simp);
                    bestOps = min(bestOps, ops);
                }
            }
        } else {
            // variant 1: swapped x<->y : (a1*y + b1*x)*(a2*y + b2)
            int A = P.c[0][2]; // c_y2
            int B = P.c[1][1]; // c_xy
            int C = P.c[0][1]; // c_y
            int D = P.c[1][0]; // c_x
            auto aPairs = factorPairs(A);
            auto bPairs = factorPairs(D); // b1*b2 = c_x
            for(auto ap : aPairs){
                int a1 = ap.first, a2 = ap.second;
                for(auto bp : bPairs){
                    int b1 = bp.first, b2 = bp.second;
                    if(a2 * b1 != B) continue;
                    if(a1 * b2 != C) continue;
                    // expansion good
                    Poly factPart;
                    factPart.c[0][2] = a1*a2;
                    factPart.c[1][1] = a2*b1;
                    factPart.c[0][1] = a1*b2;
                    factPart.c[1][0] = b1*b2;
                    Poly leftover;
                    for(int i=0;i<=2;i++) for(int j=0;j<=2;j++) leftover.c[i][j] = P.c[i][j] - factPart.c[i][j];
                    // Build AST
                    // left = a1*y + b1*x
                    auto left = make_shared<Node>(N_ADD); left->ch.clear();
                    if(a1 != 0){
                        if(a1==1) left->ch.push_back(make_shared<Node>('y'));
                        else {
                            auto m = make_shared<Node>(N_MUL);
                            m->ch.push_back(make_shared<Node>((long long)a1));
                            m->ch.push_back(make_shared<Node>('y'));
                            left->ch.push_back(m);
                        }
                    }
                    if(b1 != 0){
                        if(b1==1) left->ch.push_back(make_shared<Node>('x'));
                        else {
                            auto m = make_shared<Node>(N_MUL);
                            m->ch.push_back(make_shared<Node>((long long)b1));
                            m->ch.push_back(make_shared<Node>('x'));
                            left->ch.push_back(m);
                        }
                    }
                    if(left->ch.empty()) left = make_shared<Node>((long long)0);
                    // right = a2*y + b2
                    auto right = make_shared<Node>(N_ADD); right->ch.clear();
                    if(a2 != 0){
                        if(a2==1) right->ch.push_back(make_shared<Node>('y'));
                        else {
                            auto m = make_shared<Node>(N_MUL);
                            m->ch.push_back(make_shared<Node>((long long)a2));
                            m->ch.push_back(make_shared<Node>('y'));
                            right->ch.push_back(m);
                        }
                    }
                    if(b2 != 0) right->ch.push_back(make_shared<Node>((long long)b2));
                    if(right->ch.empty()) right = make_shared<Node>((long long)0);
                    auto factorNode = make_shared<Node>(N_MUL);
                    factorNode->ch = { left, right };
                    auto leftoverNode = buildASTFromPoly(leftover);
                    auto top = make_shared<Node>(N_ADD);
                    top->ch = { factorNode, leftoverNode };
                    auto simp = simplify(top);
                    long long ops = countOps(simp);
                    bestOps = min(bestOps, ops);
                }
            }
        }
    }

    cout << bestOps << "\n";
    return 0;
}
