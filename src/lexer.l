%{
%}

ID [0-9]+

%x IdProcCtrl1 IdProcCtrl2

%%
<INITIAL>"listar"       {BEGIN(IdProcCtrl1);}
<INITIAL>"sumar"        {BEGIN(IdProcCtrl1);}
<INITIAL>"restar"       {BEGIN(IdProcCtrl1);}
<INITIAL>"suspender"    {BEGIN(IdProcCtrl1);}
<INITIAL>"restablecer"  {BEGIN(IdProcCtrl1);}
<INITIAL>"indefinir"    {BEGIN(IdProcCtrl1);}
<INITIAL>"definir"      {BEGIN(IdProcCtrl1);}
<INITIAL>"terminar"     {BEGIN(IdProcCtrl1);}
<INITIAL>.             {printf("No hubo nada\n");}
<IdProcCtrl1>{ID}       {printf("LLegué\n");}
