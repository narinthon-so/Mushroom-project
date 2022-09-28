Imports System.Web.Script.Serialization

Public Class ConfigURL
    Private Sub Label3_Click(sender As Object, e As EventArgs) Handles Label3.Click
        TextBox1.Clear()
    End Sub

    Private Sub Label4_Click(sender As Object, e As EventArgs) Handles Label4.Click
        TextBox2.Clear()
    End Sub

    Private Sub Button2_Click(sender As Object, e As EventArgs) Handles Button2.Click
        Me.Close()
    End Sub

    Private Sub Button1_Click(sender As Object, e As EventArgs) Handles Button1.Click
        If TextBox1.TextLength And TextBox2.TextLength > 0 Then
            My.Settings.config_database_url = TextBox1.Text
            My.Settings.config_webserver_url = TextBox2.Text
            My.Settings.Save()
            MessageBox.Show("seved.", "Alert", MessageBoxButtons.OK, MessageBoxIcon.Information)
        Else
            MessageBox.Show("can't save empty values.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error)
        End If
    End Sub

    Private Sub ConfigURL_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        LabelWebUrl.Visible = False
        LabelDatabaseUrl.Visible = False
    End Sub

    Private Sub Button3_Click(sender As Object, e As EventArgs) Handles Button3.Click

        Try
            'GET URL
            Dim webClient As New System.Net.WebClient
            Dim json As String = webClient.DownloadString(My.Settings.pi_local_addr + "public-url.php")

            'JSON DECODE
            Dim jss As New JavaScriptSerializer
            Dim obj As Object = jss.Deserialize(Of Object)(json)

            My.Settings.config_webserver_url = obj("web_server")
            My.Settings.config_database_url = obj("database_server")
            My.Settings.Save()

            LabelWebUrl.Text = My.Settings.config_webserver_url
            LabelDatabaseUrl.Text = My.Settings.config_database_url
            MessageBox.Show("Successful.", "Info", MessageBoxButtons.OK, MessageBoxIcon.Information)
            'LabelWebUrl.Text = json.Length
        Catch ex As Exception
            MessageBox.Show("Error, Check connnection.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error)
            'LabelWebUrl.Text = "Error."
        End Try

        Me.Close()

    End Sub
End Class