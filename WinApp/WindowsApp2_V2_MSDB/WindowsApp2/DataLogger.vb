Public Class DataLogger
    Private Sub DataLogger_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        'TODO: This line of code loads data into the 'MushroomDataSet.dht22' table. You can move, or remove it, as needed.
        Me.Dht22TableAdapter.Fill(Me.MushroomDataSet.dht22)

        With DataGridView1
            .ClearSelection()
            .ReadOnly = True
            .MultiSelect = False
        End With

    End Sub

    Private Sub DateTimePicker1_ValueChanged(sender As Object, e As EventArgs) Handles DateTimePicker1.ValueChanged
        Me.Dht22BindingSource.Filter = "save_date LIKE '" & DateTimePicker1.Value.Date & "'"
    End Sub

    Private Sub ButtonReset_Click(sender As Object, e As EventArgs) Handles ButtonReset.Click
        Me.Dht22BindingSource.Filter = Nothing

        With DataGridView1
            .ClearSelection()
            .ReadOnly = True
            .MultiSelect = False
            .DataSource = Dht22BindingSource
        End With
    End Sub
End Class