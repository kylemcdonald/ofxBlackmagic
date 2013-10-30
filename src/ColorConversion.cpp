#include "ColorConversion.h"

unsigned char clamp(int value) {
	if(value > 255) return 255;
	if(value < 0)   return 0;
	return value;
}

bool yuvReady = false;
unsigned char yuvRed[256][256];
unsigned char yuvGreen[256][256][256];
unsigned char yuvBlue[256][256];
void createYuvLookupTables(){
	if(yuvReady) {
		return;
	}
    int yy, uu, vv, ug_plus_vg, ub, vr, val;
	for (int y = 0; y < 256; y++) {
        for (int v = 0; v < 256; v++) {
            yy = y << 8;
            vv = v - 128;
            vr = vv * 359;
            val = (yy + vr) >>  8;
            yuvRed[v][y]  = clamp(val);
        }
    }
    for (int y = 0; y < 256; y++) {
        for (int u = 0; u < 256; u++) {
            for (int v = 0; v < 256; v++) {
                yy = y << 8;
                uu = u - 128;
                vv = v - 128;
                ug_plus_vg = uu * 88 + vv * 183;
                val = (yy - ug_plus_vg) >> 8;
                yuvGreen[u][v][y] = clamp(val);
            }
        }
    }
	for (int y = 0; y < 256; y++) {
        for (int u = 0; u < 256; u++) {
            yy = y << 8;
            uu = u - 128;
            ub = uu * 454;
            val = (yy + ub) >> 8;
            yuvBlue[u][y] = clamp(val);
        }
	}
	yuvReady = true;
}

void cby0cry1_to_y(unsigned char* cby0cry1, unsigned char* y, unsigned int n) {
	for(unsigned int i = 0, j = 1; i < n; i++, j += 2) {
		y[i] = cby0cry1[j];
	}
}

void cby0cry1_to_rgb(unsigned char* cby0cry1, unsigned char* rgb, unsigned int n) {
	createYuvLookupTables();
    unsigned char u, y0, v, y1;
	unsigned int cby0cry1n = 2 * n;
    for(unsigned int i=0, j=0; i<cby0cry1n; i+=4, j+=6){
        u = cby0cry1[i+0];
        y0 = cby0cry1[i+1];
        v = cby0cry1[i+2];
        y1 = cby0cry1[i+3];
        rgb[j+0] = yuvRed[v][y0];
        rgb[j+3] = yuvRed[v][y1];
        rgb[j+1] = yuvGreen[u][v][y0];
        rgb[j+4] = yuvGreen[u][v][y1];
        rgb[j+2] = yuvBlue[u][y0];
        rgb[j+5] = yuvBlue[u][y1];
    }
}