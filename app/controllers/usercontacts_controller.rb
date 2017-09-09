class UsercontactsController < ApplicationController
  before_action :set_usercontact, only: [:show, :edit, :update, :destroy]

  # GET /usercontacts
  # GET /usercontacts.json
  def index
    @usercontacts = Usercontact.all
  end

  # GET /usercontacts/1
  # GET /usercontacts/1.json
  def show
  end

  # GET /usercontacts/new
  def new
    @usercontact = Usercontact.new
  end

  # GET /usercontacts/1/edit
  def edit
  end

  # POST /usercontacts
  # POST /usercontacts.json
  def create
    @usercontact = Usercontact.new(usercontact_params)

    respond_to do |format|
      if @usercontact.save
        format.html { redirect_to @usercontact, notice: 'Usercontact was successfully created.' }
        format.json { render :show, status: :created, location: @usercontact }
      else
        format.html { render :new }
        format.json { render json: @usercontact.errors, status: :unprocessable_entity }
      end
    end
  end

  # PATCH/PUT /usercontacts/1
  # PATCH/PUT /usercontacts/1.json
  def update
    respond_to do |format|
      if @usercontact.update(usercontact_params)
        format.html { redirect_to @usercontact, notice: 'Usercontact was successfully updated.' }
        format.json { render :show, status: :ok, location: @usercontact }
      else
        format.html { render :edit }
        format.json { render json: @usercontact.errors, status: :unprocessable_entity }
      end
    end
  end

  # DELETE /usercontacts/1
  # DELETE /usercontacts/1.json
  def destroy
    @usercontact.destroy
    respond_to do |format|
      format.html { redirect_to usercontacts_url, notice: 'Usercontact was successfully destroyed.' }
      format.json { head :no_content }
    end
  end

  private
    # Use callbacks to share common setup or constraints between actions.
    def set_usercontact
      @usercontact = Usercontact.find(params[:id])
    end

    # Never trust parameters from the scary internet, only allow the white list through.
    def usercontact_params
      params.require(:usercontact).permit(:imei, :contacts)
    end
end
