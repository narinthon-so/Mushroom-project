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

    End Sub
End Class