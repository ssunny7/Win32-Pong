#include<windows.h>
#include<ctime>

const char g_szClassName[]="myWindowClass";

RECT pd1,pd2,ball;
RECT s1,s2;
int c1,c2,dx,dy;
BOOL set,start;
char sc1[5],sc2[5];
const int del=5,ID_TIMER=1;

void DrawObjects(HDC hdc,RECT *r)
{
  HDC hdcbuf=CreateCompatibleDC(hdc);
  HBITMAP hbmBuf=CreateCompatibleBitmap(hdc,r->right,r->bottom);
  HBITMAP hbmBufOld=(HBITMAP)SelectObject(hdcbuf,hbmBuf);
  HPEN hpOld;
  HBRUSH hbrOld;
//  SetViewportOrgEx(hdcbuf,(r->right)/2,(r->bottom)/2,NULL);
  
  FillRect(hdcbuf,r,(HBRUSH)GetStockObject(BLACK_BRUSH));
  
  SetTextColor(hdcbuf,RGB(10,152,14));                           
  SetBkColor(hdcbuf,RGB(0,0,0));
  TextOut(hdcbuf,s1.left,s1.top,sc1,wsprintf(sc2,"%d",c1));
  TextOut(hdcbuf,s2.left,s2.top,sc2,wsprintf(sc1,"%d",c2));
  
  SelectObject(hdcbuf,(HPEN)GetStockObject(NULL_PEN));                                                          
  hbrOld=(HBRUSH)SelectObject(hdcbuf,CreateSolidBrush(RGB(255,0,0)));
  Rectangle(hdcbuf,pd1.left,pd1.top,pd1.right,pd1.bottom);
  SelectObject(hdcbuf,hbrOld);
  hbrOld=(HBRUSH)SelectObject(hdcbuf,CreateSolidBrush(RGB(0,0,255)));  
  Rectangle(hdcbuf,pd2.left,pd2.top,pd2.right,pd2.bottom);
  SelectObject(hdcbuf,hbrOld);  
  SelectObject(hdcbuf,(HPEN)GetStockObject(BLACK_PEN));                                                            
  
  HPEN greenPen=CreatePen(PS_SOLID,1,RGB(0,255,0));
  hpOld=(HPEN)SelectObject(hdcbuf,greenPen);
  MoveToEx(hdcbuf,r->right/2,0,NULL);
  LineTo(hdcbuf,r->right/2,r->bottom);
  SelectObject(hdcbuf,hpOld);
                   
  SelectObject(hdcbuf,CreateSolidBrush(RGB(255,50,170)));
  Ellipse(hdcbuf,ball.left,ball.top,ball.right,ball.bottom);
  
  BitBlt(hdc,0,0,r->right,r->bottom,hdcbuf,0,0,SRCCOPY);
                      
  SelectObject(hdcbuf,(HBRUSH)GetStockObject(WHITE_BRUSH));
  SelectObject(hdcbuf,hbmBufOld);
  DeleteDC(hdcbuf);
  DeleteObject(hbmBuf);
  DeleteObject(greenPen);
}

void UpdateObjects(RECT *r)
{
  int hdir;
  srand(time(0));  

  if(start==TRUE)
  {
    hdir=((int)rand)%257;
    if(set==TRUE)
    {
     if(hdir>100)
      dy=-del;
     else
      dy=del;

     set=FALSE;
    }
     
    ball.left+=dx;
    ball.top+=dy;
    ball.right+=dx;
    ball.bottom+=dy;    
    
    if(ball.top<=0)
    {
      dy=del;
      ball.top=0;
      ball.bottom=14;
    }
    if(ball.bottom>=(r->bottom))
    {
      dy=-del;
      ball.top=(r->bottom)-14;
      ball.bottom=(r->bottom);      
    }
    if(ball.left<=pd1.right && ball.top>=pd1.top && ball.bottom<=pd1.bottom)
    {
      dx=del;
      ball.left=pd1.right;
      ball.right=ball.left+14;
    }
    if(ball.right>=pd2.left && ball.top>=pd2.top && ball.bottom<=pd2.bottom)
    {
      dx=-del;
      ball.right=pd2.left;
      ball.left=ball.right-14;
    }
    if(ball.left<=0)
    {
      dx=del;
      c1++;
      start=FALSE;
      ball.left=-7+(r->right)/2;
      ball.right=7+(r->right)/2;
      ball.bottom=7+(r->bottom)/2;
      ball.top=-7+(r->bottom)/2;
    }
    if(ball.right>=((r->right)))
    {
      dx=-del;
      c2++;
      start=FALSE;
      ball.left=-7+(r->right)/2;
      ball.right=7+(r->right)/2;
      ball.bottom=7+(r->bottom)/2;
      ball.top=-7+(r->bottom)/2;
    }    
   }
}

LRESULT CALLBACK WndProc(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp)
{
	static int cxClient,cyClient;
	static int iLpdpos,iRpdpos;
	switch(msg)
	{
          case WM_CREATE:{
                          c1=0;
                          c2=0;
                          
                          dx=del;
                          dy=-del;
                                                    
                          set=FALSE;
                          start=FALSE;
                          
                          SetTimer(hwnd,ID_TIMER,15,NULL);
                         }
                         break;
          case WM_KEYDOWN:{
                           switch(wp)
                           {
                             case VK_SPACE:{
                                            RECT rc;
                                            GetClientRect(hwnd,&rc);
                                            
                                            if(start==FALSE)
                                            {
                                             start=TRUE;
                                             UpdateObjects(&rc);
                                            
                                             set=TRUE;
                                            }
                                           }
                                           break;
                             case VK_UP:{
                                         iLpdpos-=20;
                                         pd1.top=iLpdpos;
                                         pd1.bottom=iLpdpos+60;                                         
                                         if(pd1.top<=0)
                                         {
                                          iLpdpos=0;
                                          pd1.top=iLpdpos;
                                          pd1.bottom=pd1.top+60;
                                         }
                                        }
                                        break;
                             case VK_DOWN:{
                                           iLpdpos+=20;
                                           pd1.top=iLpdpos;
                                           pd1.bottom=iLpdpos+60;                                         
                                           if(pd1.bottom>=cyClient)
                                           {
                                            iLpdpos=cyClient-60;
                                            pd1.bottom=iLpdpos+60;
                                            pd1.top=iLpdpos;
                                           }
                                          }
                                          break;
                             case '\x41':{
                                           iRpdpos-=20;
                                           pd2.top=iRpdpos;
                                           pd2.bottom=iRpdpos+60;                                         
                                           if(pd2.top<=0)
                                           {
                                            iRpdpos=0;
                                            pd2.bottom=iRpdpos+60;
                                            pd2.top=iRpdpos;
                                           }
                                          }
                                          break;
                             case '\x5A':{
                                           iRpdpos+=20;
                                           pd2.top=iRpdpos;
                                           pd2.bottom=iRpdpos+60;                                         
                                           if(pd2.bottom>=cyClient)
                                           {
                                            iRpdpos=cyClient-60;
                                            pd2.bottom=iRpdpos+60;
                                            pd2.top=iRpdpos;
                                           }
                                          }
                                          break;
                           }
                          }
                          break;
          case WM_CLOSE:DestroyWindow(hwnd);
                        KillTimer(hwnd,ID_TIMER);
                        break;
 	 case WM_DESTROY:PostQuitMessage(0);
		        break;
          case WM_TIMER:{
                         HDC hdc=GetDC(hwnd);
                         RECT rc;
                         
                         GetClientRect(hwnd,&rc);
                         
                         UpdateObjects(&rc);
                         DrawObjects(hdc,&rc);
                         
                         ReleaseDC(hwnd,hdc);
                        }
                        break;
	 case WM_SIZE:{
	              cxClient=LOWORD(lp);
		     cyClient=HIWORD(lp);
                       
                       iLpdpos=-30+cyClient/2;
                       iRpdpos=-30+cyClient/2;
                       
                	     pd1.bottom=30+cyClient/2;
		     pd1.left=0;
		     pd1.right=10;
		     pd1.top=-30+cyClient/2;

		     pd2.bottom=30+cyClient/2;
		     pd2.left=cxClient-10;
		     pd2.right=cxClient;
		     pd2.top=-30+cyClient/2;

		     ball.bottom=-7+cyClient/2;
		     ball.left=-7+cxClient/2;
		     ball.right=7+cxClient/2;
		     ball.top=7+cyClient/2;
					
		     s1.left=cxClient/4-5;
		     s1.top=15;
		     s1.right=cxClient/4+15;
		     s1.bottom=cyClient-35;

		     s2.left=3*(cxClient/4)-15;
		     s2.top=0+15;
		     s2.right=3*(cxClient/4)+15;
		     s2.bottom=cyClient-35;
    	             }
		    break;
	 case WM_PAINT:{
		       HDC hdc;
            	       PAINTSTRUCT ps;
		       hdc=BeginPaint(hwnd,&ps);
		       RECT rc;
			   
		       GetClientRect(hwnd,&rc);
                         //SetViewportOrgEx(hdc,cxClient/2,cyClient/2,NULL);                            
                         DrawObjects(hdc,&rc);

		       EndPaint(hwnd,&ps);
    		      }
		      break;
	 default:return DefWindowProcA(hwnd,msg,wp,lp);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	WNDCLASSEXA wc;
	HWND hwnd;
	MSG msg;

	wc.cbSize=sizeof(WNDCLASSEX);
	wc.cbClsExtra=0;
	wc.cbWndExtra=0;
	wc.hbrBackground=(HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wc.hCursor=LoadCursor(NULL,IDC_ARROW);
	wc.hIcon=LoadIcon(hInstance,MAKEINTRESOURCE(101));
	wc.hIconSm=LoadIcon(hInstance,MAKEINTRESOURCE(101));
	wc.hInstance=hInstance;
	wc.lpszClassName="myWindowClass";
	wc.style=CS_HREDRAW|CS_VREDRAW;
	wc.lpfnWndProc=WndProc;
	wc.lpszMenuName=NULL;

	if(!RegisterClassExA(&wc))
	{
		MessageBoxA(0,"Problem Registering Class!","Error!",MB_OK|MB_ICONEXCLAMATION);
		return 0;
	}

	hwnd=CreateWindowExA(0,g_szClassName,"Pong!",WS_OVERLAPPED|WS_SYSMENU|WS_MINIMIZEBOX,CW_USEDEFAULT,CW_USEDEFAULT,700,500,NULL,NULL,hInstance,NULL);
	if(hwnd==NULL)
	{
		MessageBoxA(0,"Problem Creating Window!","Error!",MB_OK|MB_ICONEXCLAMATION);
		return 0;
	}

	ShowWindow(hwnd,nCmdShow);
	UpdateWindow(hwnd);

	while(GetMessage(&msg,hwnd,0,0)>0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}
