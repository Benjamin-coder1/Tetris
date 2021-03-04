


class Couleur {

	private :
		int R;
		int G;
		int B;

	public :
		Couleur () {};
		Couleur(int r,int g, int b): R(r),G(g),B(b) {};
		Couleur(Couleur const &col): R(col.R),G(col.G),B(col.B) {}

		int getR() {
			return R;
		};
		int getG() {
			return G;
		};
		int getB() {
			return B;
		};
};