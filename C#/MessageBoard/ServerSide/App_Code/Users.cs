using System;
using System.Data;
using System.Configuration;
using System.Web;
using System.Web.Security;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Web.UI.WebControls.WebParts;
using System.Web.UI.HtmlControls;

/// <summary>
/// Summary description for Users
/// </summary>
public class Users
{
    private string UserName; //�� �����
    private string Password; //�����
    private string FirstName; //�� ����
    private string LastName; //�� �����
    private string Email;
    private int Rank; //����
    private int Status; //�����
    
	public Users()
	{
       
	} //����� ����

    public string UserNames
    {
        get
        {
            return this.UserName;
        }
        set
        {
            this.UserName = value;
        }
        
    } //UserName ������ ����� �/�� ���� �� ��� ������

    public string Passwords
    {
        get
        {
            return this.Password;
        }
        set
        {
            this.Password = value;
        }

    } //Password ������ ����� �/�� ���� �� ��� ������

    public string FirstNames
    {
        get
        {
            return this.FirstName;
        }
        set
        {
            this.FirstName = value;
        }

    } //FirstName ������ ����� �/�� ���� �� ��� ������

    public string LastNames
    {
        get
        {
            return this.LastName;
        }
        set
        {
            this.LastName = value;
        }

    } //LastName ������ ����� �/�� ���� �� ��� ������

    public string Emails
    {
        get
        {
            return this.Email;
        }
        set
        {
            this.Email = value;
        }
    }

    public int Ranks
    {
        get
        {
            return this.Rank;
        }
        set
        {
            this.Rank = value;
        }
    } //Rank ������ ����� �/�� ���� �� ��� ������

    public int Statuss
    {
        get
        {
            return this.Status;
        }
        set
        {
            this.Status = value;
        }
    } //Status ������ ����� �/�� ���� �� ��� ������
}
