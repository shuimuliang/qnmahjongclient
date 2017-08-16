{************************************************************************}
{                                                                        }
{            单元名：frm_main.pas                                        }
{            功能：基于WxSQLite密码操作API的SQLite加密、清空密码、更改密 }
{                  码的操作。                                            }
{            作者：Pfeng （技术博客：www.pfeng.org）                     }
{            日期：2011.12.08                                            }
{                                                                        }
{************************************************************************}
unit frm_main;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, ComCtrls, frxpngimage, ExtCtrls,Shellapi;

type
  TSQLiteDB = Pointer;  
  TSqlite3 = Pointer;
  TForm1 = class(TForm)
    OpenDialog1: TOpenDialog;
    GroupBox1: TGroupBox;
    EDT_dbpath: TEdit;
    GroupBox2: TGroupBox;
    EDT_dbopwd: TEdit;
    Label1: TLabel;
    Label2: TLabel;
    Label3: TLabel;
    EDT_dbnpwd: TEdit;
    EDT_dbnpwd2: TEdit;
    BTN_save: TButton;
    Label4: TLabel;
    BTN_open: TButton;
    StatusBar1: TStatusBar;
    Image1: TImage;
    procedure BTN_openClick(Sender: TObject);
    procedure BTN_saveClick(Sender: TObject);
    procedure Image1Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

const
  SQLiteDLL = 'sqlite3.dll';

var
  Form1: TForm1;
  function SQLite3_Open(filename: PAnsiChar; var db: TSQLiteDB): integer; cdecl; external SQLiteDLL name 'sqlite3_open';
  function sqlite3_key (
    pDb: Tsqlite3;          // Database handle  
    pKey: PAnsiChar;        // Database PassWord (UTF-8)  
    nKey: Integer           // Database sizeofpassword
  ): integer;  cdecl; external SQLiteDLL name 'sqlite3_key';
  function sqlite3_rekey (
    pDb: Tsqlite3;          // Database handle  
    pKey: PAnsiChar;        // Database PassWord (UTF-8)
    nKey: Integer           // Database sizeofpassword
  ):integer;  cdecl; external SQLiteDLL name 'sqlite3_rekey';
  function SQLite3_Close(db: TSQLiteDB): integer; cdecl; external SQLiteDLL name 'sqlite3_close';
implementation

{$R *.dfm}

function IsAllEnglishStr(const Str :string):Boolean;
var
 i: Integer;
begin
  {判断是否全部是英文或数字}
  Result := True;
  for i:=0 to Length(Str)-1 do
  begin
    if ord(Str[i]) > 128 then
    begin
      Result := False;
      Exit;
    end;
  end;
end;

function TranslateDBFile(Str: string):string;
begin
   {因SQLITE路径必须为宽字节，因此汉字路径需要转换一下}
  if IsAllEnglishStr(Str) then
    Result := Str
  else
    Result := Utf8Encode(Str);
end;

procedure TForm1.BTN_openClick(Sender: TObject);
begin
  {打开数据库文件}
  if OpenDialog1.Execute then
  begin
    EDT_dbpath.Text:=OpenDialog1.FileName;
  end;
end;

procedure TForm1.BTN_saveClick(Sender: TObject);
var
 flag: integer;
 db: TSQLiteDB;
begin
  {修改密码}
  if not FileExists(Trim(EDT_dbpath.Text)) then
  begin
    Application.MessageBox('SQLite数据库不存在，请重新选择！', '错误', MB_OK +
      MB_ICONSTOP);
    Exit;
  end;
  if Trim(EDT_dbnpwd.Text) <> Trim(EDT_dbnpwd2.Text) then
  begin
    Application.MessageBox('两次新密码不一致，请重新设置！', '错误', MB_OK +
      MB_ICONSTOP);
    Exit;
  end;
  if (Trim(EDT_dbopwd.Text) = '') and (Trim(EDT_dbnpwd.Text) = '') then
  begin
    //禁止都原始密码和新密码都为空时的操作，否则API返回值均为0（有兴趣的可以测试一下）
    Exit;
  end;
  SQLite3_Open(Pansichar(TranslateDBFile(Trim(EDT_dbpath.Text))),db);//打开数据库
  flag := SQLite3_key(db,Pansichar(Trim(EDT_dbopwd.Text)),Length(Trim(EDT_dbopwd.Text))); //验证密码
  if flag <> 0 then
  begin
    Application.MessageBox(Pansichar('数据库初始密码验证失败！(SQLite3_key:'+inttostr(flag)+')'), '错误', MB_OK +
      MB_ICONSTOP);
    Exit;
  end;
  flag := SQLite3_rekey(db,Pansichar(Trim(EDT_dbnpwd.Text)),Length(Trim(EDT_dbnpwd.Text))); //更改密码
  if flag <> 0 then
  begin
    Application.MessageBox(Pansichar('数据库密码修改失败！(SQLite3_rekey:'+inttostr(flag)+')'), '错误', MB_OK +
      MB_ICONSTOP);
    Exit;
  end;
  SQLite3_Close(db);//关闭数据库
  Application.MessageBox('数据库密码应用成功！', '提示', MB_OK +
      MB_ICONinformation);
end;

procedure TForm1.Image1Click(Sender: TObject);
begin
  ShellExecute(Handle,'open','www.pfeng.org',nil,nil,SW_SHOWNORMAL)
end;

end.
