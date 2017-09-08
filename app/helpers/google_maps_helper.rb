module GoogleMapsHelper
	def google_map(center)
	  "https://maps.googleapis.com/maps/api/staticmap?center=#{center}&size=300x300&zoom=17&key=AIzaSyCxGbUAAoZuQDgMkjzXxKYchQLvqL5WRqc"
	end
	# def dynamic_map()
	# 	"<iframe width="600" height="450" frameborder="0" style="border:10"
	# 	 src="https://maps.googleapis.com/maps/api/staticmap?center=12.923047,77.498950&zoom=10&size=100x100&key=AIzaSyCxGbUAAoZuQDgMkjzXxKYchQLvqL5WRqc"
	# 	 allowfullscreen></iframe>"
	# end
end