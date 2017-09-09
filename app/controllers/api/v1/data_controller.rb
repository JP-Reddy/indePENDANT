class Api::V1::DataController < ApiController
  before_action :set_params, only: [:create]
  # respond_to :json
  # GET /hyperloop/v1/user_clicks
  # GET /hyperloop/v1/user_clicks.json
  # def index
  #   @clicks = Product.all
  #   respond_with(@clicks)
  # end

  def index
     render json: { status: "Test API"}
  end

  def create
       puts params[:imei]
       puts params[:contact]

      render json: { status: "created"}
  end

  private
   
   def set_params
      @user_imei = params[:imei]
   end
end