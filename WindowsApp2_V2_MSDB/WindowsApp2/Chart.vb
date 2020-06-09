Public Class Chart

    Private Sub Chart_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        'TODO: This line of code loads data into the 'MushroomDataSet.dht22' table. You can move, or remove it, as needed.
        Me.Dht22TableAdapter.Fill(Me.MushroomDataSet.dht22)

        Me.Dht22BindingSource.Filter = "save_date LIKE '" & Date.Now.ToString("d/M/yyyy") & "'"

        'setup chart
        Chart1.ChartAreas(0).AxisX.LabelStyle.Interval = 1

        ' Axis X 
        Chart1.ChartAreas(0).AxisX.TitleFont = New Font("Oswald", 18.0F, FontStyle.Regular, GraphicsUnit.Pixel)
        Chart1.ChartAreas(0).AxisX.Title = "Save Time"

        ' Axis Y
        Chart1.ChartAreas(0).AxisY.TitleFont = New Font("Oswald", 18.0F, FontStyle.Regular, GraphicsUnit.Pixel)
        Chart1.ChartAreas(0).AxisY.Title = "Temperature and Humidity"

        ' Handle legend label
        Chart1.Series("Temperature").IsVisibleInLegend = True
        Chart1.Series("Humidity").IsVisibleInLegend = True

        ' Show number value in bar / column chart
        Chart1.Series("Temperature").IsValueShownAsLabel = True
        Chart1.Series("Humidity").IsValueShownAsLabel = True

        ' Value member
        Chart1.Series("Temperature").XValueMember = "save_time"
        Chart1.Series("Temperature").YValueMembers = "temperature"
        Chart1.Series("Humidity").XValueMember = "save_time"
        Chart1.Series("Humidity").YValueMembers = "humidity"

    End Sub

    Private Sub DateTimePicker1_ValueChanged(sender As Object, e As EventArgs) Handles DateTimePicker1.ValueChanged

        Me.Dht22BindingSource.Filter = "save_date LIKE '" & DateTimePicker1.Value.Date & "'"

        'setup chart
        Chart1.ChartAreas(0).AxisX.LabelStyle.Interval = 1

        ' Axis X 
        Chart1.ChartAreas(0).AxisX.TitleFont = New Font("Oswald", 18.0F, FontStyle.Regular, GraphicsUnit.Pixel)
        Chart1.ChartAreas(0).AxisX.Title = "Save Time"

        ' Axis Y
        Chart1.ChartAreas(0).AxisY.TitleFont = New Font("Oswald", 18.0F, FontStyle.Regular, GraphicsUnit.Pixel)
        Chart1.ChartAreas(0).AxisY.Title = "Temperature and Humidity"

        ' Handle legend label
        Chart1.Series("Temperature").IsVisibleInLegend = True
        Chart1.Series("Humidity").IsVisibleInLegend = True

        ' Show number value in bar / column chart
        Chart1.Series("Temperature").IsValueShownAsLabel = True
        Chart1.Series("Humidity").IsValueShownAsLabel = True

        ' Value member
        Chart1.Series("Temperature").XValueMember = "save_time"
        Chart1.Series("Temperature").YValueMembers = "temperature"
        Chart1.Series("Humidity").XValueMember = "save_time"
        Chart1.Series("Humidity").YValueMembers = "humidity"
    End Sub
End Class