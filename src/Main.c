#include "/home/codeleaded/System/Static/Library/WindowEngine.h"
#include "/home/codeleaded/System/Static/Library/Splines.h"
#include "/home/codeleaded/System/Static/Library/TransformedView.h"
#include "/home/codeleaded/System/Static/Library/ObjectNotation.h"

#define SHAPE_CTLPOINT_LX   1.0f
#define SHAPE_CTLPOINT_LY   1.0f

#define SHAPE_NONE      0
#define SHAPE_RECT      1
#define SHAPE_LINE      2
#define SHAPE_CIRCLE    3
#define SHAPE_SPLINE    4

typedef unsigned int Shape_Type;

typedef struct SRect {
    Vec2 p;
    Vec2 e;
} SRect;

void SRect_Update(SRect* r,Vec2** out,Vec2 m){
    const Vec2 l = (Vec2){ SHAPE_CTLPOINT_LX,SHAPE_CTLPOINT_LY };
    if(Overlap_Rect_Point((Rect){ Vec2_Sub(r->p,Vec2_Mulf(l,0.5f)),l },m))      *out = &r->p;
	else if(Overlap_Rect_Point((Rect){ Vec2_Sub(r->e,Vec2_Mulf(l,0.5f)),l },m)) *out = &r->e;
}
void SRect_Render(SRect* r,unsigned int* Target,int Width,int Height,TransformedView* tv){
	const Rect ScreenRect = TransformedView_ScreenWorldRect(tv,(Rect){ 0.0f,0.0f,GetWidth(),GetHeight() });
    const Rect rect = {
        .p = r->p,
        .d = (Vec2){ F32_Abs(r->e.x - r->p.x),F32_Abs(r->e.y - r->p.y) }
    };

    if(Overlap_Rect_Rect(ScreenRect,rect)){
		const Vec2 p = TransformedView_WorldScreenPos(tv,(Vec2){ F32_Min(r->p.x,r->e.x),F32_Min(r->p.y,r->e.y) });
		const Vec2 d = TransformedView_WorldScreenLength(tv,(Vec2){ F32_Abs(r->e.x - r->p.x),F32_Abs(r->e.y - r->p.y) });
        Rect_RenderXWire(Target,Width,Height,p,d,RED,1.0f);
        
        const Vec2 l = TransformedView_WorldScreenLength(tv,(Vec2){ SHAPE_CTLPOINT_LX,SHAPE_CTLPOINT_LY });
		const Vec2 sp = TransformedView_WorldScreenPos(tv,r->p);
		const Vec2 se = TransformedView_WorldScreenPos(tv,r->e);

        const Vec2 lp = Vec2_Sub(sp,Vec2_Mulf(l,0.5f));
        const Vec2 le = Vec2_Sub(se,Vec2_Mulf(l,0.5f));
        Rect_RenderX(Target,Width,Height,lp,l,GREEN);
        Rect_RenderX(Target,Width,Height,le,l,GREEN);
	}
}

typedef struct SLine {
    Vec2 p;
    Vec2 e;
} SLine;

void SLine_Update(SLine* r,Vec2** out,Vec2 m){
    const Vec2 l = (Vec2){ SHAPE_CTLPOINT_LX,SHAPE_CTLPOINT_LY };
    if(Overlap_Rect_Point((Rect){ Vec2_Sub(r->p,Vec2_Mulf(l,0.5f)),l },m))      *out = &r->p;
	else if(Overlap_Rect_Point((Rect){ Vec2_Sub(r->e,Vec2_Mulf(l,0.5f)),l },m)) *out = &r->e;
}
void SLine_Render(SLine* r,unsigned int* Target,int Width,int Height,TransformedView* tv){
	const Rect ScreenRect = TransformedView_ScreenWorldRect(tv,(Rect){ 0.0f,0.0f,GetWidth(),GetHeight() });

    const Vec2 p = TransformedView_WorldScreenPos(tv,r->p);
	const Vec2 e = TransformedView_WorldScreenPos(tv,r->e);
    const Vec2 l = TransformedView_WorldScreenLength(tv,(Vec2){ SHAPE_CTLPOINT_LX,SHAPE_CTLPOINT_LY });
    Line_RenderX(Target,Width,Height,p,e,RED,1.0f);
        
    const Vec2 lp = Vec2_Sub(p,Vec2_Mulf(l,0.5f));
    const Vec2 le = Vec2_Sub(e,Vec2_Mulf(l,0.5f));
    Rect_RenderX(Target,Width,Height,lp,l,GREEN);
    Rect_RenderX(Target,Width,Height,le,l,GREEN);
}

typedef struct SCircle {
    Vec2 p;
    Vec2 e;
} SCircle;

void SCircle_Update(SCircle* r,Vec2** out,Vec2 m){
    const Vec2 l = (Vec2){ SHAPE_CTLPOINT_LX,SHAPE_CTLPOINT_LY };
    if(Overlap_Rect_Point((Rect){ Vec2_Sub(r->p,Vec2_Mulf(l,0.5f)),l },m))      *out = &r->p;
	else if(Overlap_Rect_Point((Rect){ Vec2_Sub(r->e,Vec2_Mulf(l,0.5f)),l },m)) *out = &r->e;
}
void SCircle_Render(SCircle* r,unsigned int* Target,int Width,int Height,TransformedView* tv){
    const Rect ScreenRect = TransformedView_ScreenWorldRect(tv,(Rect){ 0.0f,0.0f,GetWidth(),GetHeight() });
    const Vec2 len = Vec2_Sub(r->e,r->p);
    const float rad = Vec2_Mag(len);
    const Vec2 tl = Vec2_Sub(r->p,(Vec2){ rad,rad });
    const Vec2 br = Vec2_Add(r->p,(Vec2){ rad,rad });

    if(Overlap_Rect_Rect(ScreenRect,(Rect){ tl,Vec2_Sub(br,tl) })){
		const Vec2 sp = TransformedView_WorldScreenPos(tv,r->p);
		const Vec2 se = TransformedView_WorldScreenPos(tv,r->e);
        const Vec2 sr = TransformedView_WorldScreenLength(tv,(Vec2){ rad,rad });
        Circle_R_RenderWire(Target,Width,Height,sp,sr,RED,1.0f);

        const Vec2 l = TransformedView_WorldScreenLength(tv,(Vec2){ SHAPE_CTLPOINT_LX,SHAPE_CTLPOINT_LY });
        const Vec2 lp = Vec2_Sub(sp,Vec2_Mulf(l,0.5f));
        const Vec2 le = Vec2_Sub(se,Vec2_Mulf(l,0.5f));
        Rect_RenderX(Target,Width,Height,lp,l,GREEN);
        Rect_RenderX(Target,Width,Height,le,l,GREEN);
	}
}

typedef BCurve SSpline;

void SSpline_Update(SSpline* r,Vec2** out,Vec2 m){
    const Vec2 l = (Vec2){ SHAPE_CTLPOINT_LX,SHAPE_CTLPOINT_LY };
    if(Overlap_Rect_Point((Rect){ Vec2_Sub(r->p1,Vec2_Mulf(l,0.5f)),l },m))      *out = &r->p1;
	else if(Overlap_Rect_Point((Rect){ Vec2_Sub(r->p2,Vec2_Mulf(l,0.5f)),l },m)) *out = &r->p2;
	else if(Overlap_Rect_Point((Rect){ Vec2_Sub(r->p3,Vec2_Mulf(l,0.5f)),l },m)) *out = &r->p3;
}
void SSpline_Render(SSpline* r,unsigned int* Target,int Width,int Height,TransformedView* tv){
	const Rect ScreenRect = TransformedView_ScreenWorldRect(tv,(Rect){ 0.0f,0.0f,GetWidth(),GetHeight() });

    const Vec2 p1 = TransformedView_WorldScreenPos(tv,r->p1);
	const Vec2 p2 = TransformedView_WorldScreenPos(tv,r->p2);
	const Vec2 p3 = TransformedView_WorldScreenPos(tv,r->p3);
    BCurve_Render(WINDOW_STD_ARGS,(BCurve[]){ BCurve_New(p1,p2,p3) },RED);
    
    const Vec2 l = TransformedView_WorldScreenLength(tv,(Vec2){ SHAPE_CTLPOINT_LX,SHAPE_CTLPOINT_LY });
    const Vec2 lp1 = Vec2_Sub(p1,Vec2_Mulf(l,0.5f));
    const Vec2 lp2 = Vec2_Sub(p2,Vec2_Mulf(l,0.5f));
    const Vec2 lp3 = Vec2_Sub(p3,Vec2_Mulf(l,0.5f));
    Rect_RenderX(Target,Width,Height,lp1,l,GREEN);
    Rect_RenderX(Target,Width,Height,lp2,l,GREEN);
    Rect_RenderX(Target,Width,Height,lp3,l,GREEN);
}

typedef struct Shape {
    union {
        SRect r;
        SLine l;
        SCircle c;
        SSpline s;
    };
    Shape_Type t;
    void(*Update)(struct Shape*,Vec2**,Vec2);
    void(*Render)(struct Shape*,unsigned int*,int,int,TransformedView*);
} Shape;



Vec2* Focused = NULL;
TransformedView tv;
Vector Objects;

void Setup(AlxWindow* w){
    tv = TransformedView_Make(
        (Vec2){ GetWidth(),GetHeight() },
        (Vec2){ -100.0f,100.0f },
        (Vec2){ 0.1f,0.1f },
        (float)GetWidth() / (float)GetHeight()
    );
    Objects = Vector_New(sizeof(Shape));

    //Vector_Push(&Objects,(Shape[]){ Shape_New(SHAPE_RECT,(Rect[]){ {100.0f,100.0f},{200.0f,200.0f} },sizeof(Rect),(void(*)(void*))SRect_Update,(void(*)(void*))SRect_Render) });
    //Vector_Push(&Objects,(Shape[]){ Shape_New(SHAPE_LINE,(SLine[]){ {300.0f,300.0f},{400.0f,400.0f} },sizeof(SLine),(void(*)(void*))SLine_Update,(void(*)(void*))SLine_Render) });
    //Vector_Push(&Objects,(Shape[]){ Shape_New(SHAPE_CIRCLE,(SCircle[]){ {500.0f,500.0f},{600.0f,400.0f} },sizeof(SCircle),(void(*)(void*))SCircle_Update,(void(*)(void*))SCircle_Render) });
    //Vector_Push(&Objects,(Shape[]){ Shape_New(SHAPE_SPLINE,(SSpline[]){ {500.0f,500.0f},{600.0f,400.0f},{600.0f,400.0f} },sizeof(SSpline),(void(*)(void*))SSpline_Update,(void(*)(void*))SSpline_Render) });
}

void Update(AlxWindow* w){
    TransformedView_HandlePanZoom(&tv,w->Strokes,GetMouse());
    const Vec2 m = TransformedView_ScreenWorldPos(&tv,GetMouse());
    
    if(Stroke(ALX_MOUSE_L).PRESSED){
        Focused = NULL;
        
        for(int i = 0;i<Objects.size;i++){
            Shape* s = (Shape*)Vector_Get(&Objects,i);
            s->Update(s,&Focused,m);
        }
    }else if(Stroke(ALX_MOUSE_L).RELEASED){
        Focused = NULL;
    }

    if(Focused){
        *Focused = m;
    }

    if(Stroke(ALX_KEY_1).PRESSED){
        Vector_Push(&Objects,(Shape[]){
            (Shape){
                .t = SHAPE_RECT,
                .r = (SRect){ .p = m, .e = m },
                .Update = (void(*)(Shape*,Vec2**,Vec2))SRect_Update,
                .Render = (void(*)(Shape*,unsigned int*,int,int,TransformedView*))SRect_Render
            }
        });
    }
    if(Stroke(ALX_KEY_2).PRESSED){
        Vector_Push(&Objects,(Shape[]){
            (Shape){
                .t = SHAPE_LINE,
                .l = (SLine){ .p = m, .e = m },
                .Update = (void(*)(Shape*,Vec2**,Vec2))SLine_Update,
                .Render = (void(*)(Shape*,unsigned int*,int,int,TransformedView*))SLine_Render
            }
        });
    }
    if(Stroke(ALX_KEY_3).PRESSED){
        Vector_Push(&Objects,(Shape[]){
            (Shape){
                .t = SHAPE_CIRCLE,
                .c = (SCircle){ .p = m, .e = m },
                .Update = (void(*)(Shape*,Vec2**,Vec2))SCircle_Update,
                .Render = (void(*)(Shape*,unsigned int*,int,int,TransformedView*))SCircle_Render
            }
        });
    }
    if(Stroke(ALX_KEY_4).PRESSED){
        Vector_Push(&Objects,(Shape[]){
            (Shape){
                .t = SHAPE_SPLINE,
                .s = (SSpline){ .p1 = m, .p2 = m, .p3 = m },
                .Update = (void(*)(Shape*,Vec2**,Vec2))SSpline_Update,
                .Render = (void(*)(Shape*,unsigned int*,int,int,TransformedView*))SSpline_Render
            }
        });
    }
    if(Stroke(ALX_KEY_5).PRESSED){
        Table t = Table_New();
        Table_Parse(&t,"./data/Objects.alxon");
        //Table_Print(&t);
        
        for(int i = 0;i<t.Root->Childs.size;i++){
            const Branch* const b = *(Branch**)Vector_Get(&t.Root->Childs,i);
            const Entry* const e = b->Memory;

            Vector out = CStr_ChopDown(e->Value,' ');
            if(CStr_Cmp(e->Name,"RECT") && out.size == 4){
                Vector_Push(&Objects,(Shape[]){
                    (Shape){
                        .t = SHAPE_RECT,
                        .r = (SRect){ .p = {
                            Double_Parse(*(char**)Vector_Get(&out,0),1),
                            Double_Parse(*(char**)Vector_Get(&out,1),1)
                        }, .e = {
                            Double_Parse(*(char**)Vector_Get(&out,2),1),
                            Double_Parse(*(char**)Vector_Get(&out,3),1)
                        }},
                        .Update = (void(*)(Shape*,Vec2**,Vec2))SRect_Update,
                        .Render = (void(*)(Shape*,unsigned int*,int,int,TransformedView*))SRect_Render
                    }
                });
            }else if(CStr_Cmp(e->Name,"LINE") && out.size == 4){
                Vector_Push(&Objects,(Shape[]){
                    (Shape){
                        .t = SHAPE_LINE,
                        .l = (SLine){ .p = {
                            Double_Parse(*(char**)Vector_Get(&out,0),1),
                            Double_Parse(*(char**)Vector_Get(&out,1),1)
                        }, .e = {
                            Double_Parse(*(char**)Vector_Get(&out,2),1),
                            Double_Parse(*(char**)Vector_Get(&out,3),1)
                        }},
                        .Update = (void(*)(Shape*,Vec2**,Vec2))SLine_Update,
                        .Render = (void(*)(Shape*,unsigned int*,int,int,TransformedView*))SLine_Render
                    }
                });
            }else if(CStr_Cmp(e->Name,"CIRCLE") && out.size == 4){
                Vector_Push(&Objects,(Shape[]){
                    (Shape){
                        .t = SHAPE_CIRCLE,
                        .c = (SCircle){ .p = {
                            Double_Parse(*(char**)Vector_Get(&out,0),1),
                            Double_Parse(*(char**)Vector_Get(&out,1),1)
                        }, .e = {
                            Double_Parse(*(char**)Vector_Get(&out,2),1),
                            Double_Parse(*(char**)Vector_Get(&out,3),1)
                        }},
                        .Update = (void(*)(Shape*,Vec2**,Vec2))SCircle_Update,
                        .Render = (void(*)(Shape*,unsigned int*,int,int,TransformedView*))SCircle_Render
                    }
                });
            }else if(CStr_Cmp(e->Name,"SPLINE") && out.size == 6){
                Vector_Push(&Objects,(Shape[]){
                    (Shape){
                        .t = SHAPE_SPLINE,
                        .s = (SSpline){ .p1 = {
                            Double_Parse(*(char**)Vector_Get(&out,0),1),
                            Double_Parse(*(char**)Vector_Get(&out,1),1)
                        }, .p2 = {
                            Double_Parse(*(char**)Vector_Get(&out,2),1),
                            Double_Parse(*(char**)Vector_Get(&out,3),1)
                        }, .p3 = {
                            Double_Parse(*(char**)Vector_Get(&out,4),1),
                            Double_Parse(*(char**)Vector_Get(&out,5),1)
                        }},
                        .Update = (void(*)(Shape*,Vec2**,Vec2))SSpline_Update,
                        .Render = (void(*)(Shape*,unsigned int*,int,int,TransformedView*))SSpline_Render
                    }
                });
            }
            
            for(int j = 0;j<out.size;j++){
                char* cstr = *(char**)Vector_Get(&out,j);
                free(cstr);
            }
            Vector_Free(&out);
        }
        
        //Table_Write(&t,"C:/Wichtig/Hecke/C/Win_Polymorphism/Objects.alxon");
        Table_Free(&t);
    }
    if(Stroke(ALX_KEY_6).PRESSED){
        Table t = Table_New();
        Branch_Set(t.Root,(Entry[]){ Entry_New("Objects",NULL,0) },sizeof(Entry));
        for(int i = 0;i<Objects.size;i++){
            Shape* s = (Shape*)Vector_Get(&Objects,i);

            switch (s->t){
                case SHAPE_RECT: {
                    String str = String_Format("%f %f %f %f",s->r.p.x,s->r.p.y,s->r.e.x,s->r.e.y);
                    char* cstr = String_CStr(&str);
                    Branch_Add(t.Root,Branch_New((Entry[]){ Entry_New("RECT",cstr,0) },sizeof(Entry)));
                    free(cstr);
                    String_Free(&str);
                } break;
                case SHAPE_LINE: {
                    String str = String_Format("%f %f %f %f",s->l.p.x,s->l.p.y,s->l.e.x,s->l.e.y);
                    char* cstr = String_CStr(&str);
                    Branch_Add(t.Root,Branch_New((Entry[]){ Entry_New("LINE",cstr,0) },sizeof(Entry)));
                    free(cstr);
                    String_Free(&str);
                } break;
                case SHAPE_CIRCLE: {
                    String str = String_Format("%f %f %f %f",s->l.p.x,s->l.p.y,s->l.e.x,s->l.e.y);
                    char* cstr = String_CStr(&str);
                    Branch_Add(t.Root,Branch_New((Entry[]){ Entry_New("CIRCLE",cstr,0) },sizeof(Entry)));
                    free(cstr);
                    String_Free(&str);
                } break;
                case SHAPE_SPLINE: {
                    String str = String_Format("%f %f %f %f %f %f",s->s.p1.x,s->s.p1.y,s->s.p2.x,s->s.p2.y,s->s.p3.x,s->s.p3.y);
                    char* cstr = String_CStr(&str);
                    Branch_Add(t.Root,Branch_New((Entry[]){ Entry_New("SPLINE",cstr,0) },sizeof(Entry)));
                    free(cstr);
                    String_Free(&str);
                } break;
                default: {
                    printf("[Main]: Update -> unkown type: %d\n",s->t);
                } break;
            }
        }
        //Table_Print(&t);
        Table_Write(&t,"./data/Objects.alxon");
        Table_Free(&t);
    }

    Clear(DARK_BLUE);

    for(int i = 0;i<Objects.size;i++){
        Shape* s = (Shape*)Vector_Get(&Objects,i);
        s->Render(s,WINDOW_STD_ARGS,&tv);
    }

    CStr_RenderAlxFontf(WINDOW_STD_ARGS,GetAlxFont(),0.0f,0.0f,WHITE,"Offset: %f,%f",tv.Offset.x,tv.Offset.y);
    CStr_RenderAlxFontf(WINDOW_STD_ARGS,GetAlxFont(),0.0f,GetAlxFont()->CharSizeY,WHITE,"Scale:  %f,%f",tv.Scale.x,tv.Scale.y);
}

void Delete(AlxWindow* w){
    Vector_Free(&Objects);
}

int main(){
    if(Create("Polymorphism Using Enum and Union",1800,1000,1,1,Setup,Update,Delete))
        Start();
    return 0;
}