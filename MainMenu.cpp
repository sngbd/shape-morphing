#include "MainMenu.h"

MainMenu::MainMenu() 
{ 
	this->titleText = Text(Vector3(250, 100, 0), Color(0, 255, 0, 255), "Shape Morphing");
	this->instructionsTitleText = Text(Vector3(10, 200, 0), Color(255, 255, 255, 255), "Petunjuk:");
	this->instructionsline1Text = Text(Vector3(20, 250, 0), Color(255, 255, 255, 255), "Anda harus menempatkan semua sudut shape yang diberikan,");
	this->instructionsline2Text = Text(Vector3(20, 275, 0), Color(255, 255, 255, 255), "Anda akan diberi waktu 5 detik untuk mempelajari bentuk sebelum");
	this->instructionsline3Text = Text(Vector3(20, 300, 0), Color(255, 255, 255, 255), "shape menghilang, kemudian Anda harus menggambar sudut-sudut");
	this->instructionsline4Text = Text(Vector3(20, 325, 0), Color(255, 255, 255, 255), "shape tersebut, semakin dekat Anda mendekati sudut yang benar,");
	this->instructionsline5Text = Text(Vector3(20, 350, 0), Color(255, 255, 255, 255), "maka skor yang diperoleh akan semakin baik.");
	this->instructionsline6Text = Text(Vector3(20, 400, 0), Color(255, 255, 255, 255), "CATATAN:");
	this->instructionsline7Text = Text(Vector3(20, 425, 0), Color(255, 255, 255, 255), "titik yang Anda gambar akan secara berurutan menuju ke yang");
	this->instructionsline8Text = Text(Vector3(20, 450, 0), Color(255, 255, 255, 255), "terdekat sehingga saya sarankan melakukannya secara berurutan,");
	this->instructionsline9Text = Text(Vector3(20, 475, 0), Color(255, 255, 255, 255), "jika tidak Anda akan kehilangan banyak poin, dan");
	this->instructionsline10Text = Text(Vector3(20, 500, 0), Color(255, 255, 255, 255), "itu tidak akan terlihat bagus ketika selesai menyatu.");
	this->instructionsline10Text = Text(Vector3(20, 525, 0), Color(255, 255, 255, 255), "(Tekan tombol panah kanan dan kiri untuk navigasi frame)");
	this->clickToStartText = Text(Vector3(200, 550, 0), Color(255, 255, 255, 255), "(Klik Untuk Melanjutkan)");

}

void MainMenu::drawMainMenu()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	const double w = glutGet(GLUT_WINDOW_WIDTH);
	const double h = glutGet(GLUT_WINDOW_HEIGHT);
	glOrtho(0, w, h, 0, -100, 100);
	
	titleText.drawText();
	instructionsTitleText.drawText();
	instructionsline1Text.drawText();
	instructionsline2Text.drawText();
	instructionsline3Text.drawText();
	instructionsline4Text.drawText();
	instructionsline5Text.drawText();
	instructionsline6Text.drawText();
	instructionsline7Text.drawText();
	instructionsline8Text.drawText();
	instructionsline9Text.drawText();
	instructionsline10Text.drawText();
	clickToStartText.drawText();
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glutSwapBuffers();
}
