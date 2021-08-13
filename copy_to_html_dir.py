import os
from datetime import datetime

#Edit this string to point to your output dir
web_folder = '~/public_html/Production/'

var = 'Plots_210813/Results'
input_folder = '/vols/cms/VBFHinv/Common/'+var + '/'

today = datetime.now()
Production_title = 'Production_'+today.strftime("%Y_%m_%d"+"_QCD")
output_location = web_folder + Production_title

Regions = ['A', 'B', 'CR', 'HFNoiseCR', 'SR', 'HFNoise']
Categories = ['MTR', 'VTR']
Eras = ['2017', '2018']

print('Processing location: ' + input_folder)
print('-- Copying files to :' + output_location)

os.system('mkdir -p ' +  output_location)
os.system('cp index.php ' + output_location)

os.system('mkdir -p ' +  output_location + '/FitResults')
os.system('cp index.php ' + output_location + '/FitResults/')

for era in Eras:
    for category in Categories:
        os.system('mkdir -p '+output_location+'/FitResults/'+category+'_'+era+'/')
        os.system('cp '+input_folder+'/*'+category+'*'+era+'*  '+ output_location+'/FitResults/'+category+'_'+era+'/')
        os.system('cp html/index_'+category+'_'+era+'.html ' + output_location + '/FitResults/' +category+'_'+era+'/index.html')
        os.system('cp index.php '+output_location+'/FitResults/'+category+'_'+era+'/')
        for region in Regions:
            os.system('mkdir -p '+output_location+'/'+category+'_'+era+'/'+region)
            os.system('cp '+input_folder+'/'+region+'/*'+category+'*'+era+'*  '+ output_location+'/'+category+'_'+era+'/'+'/'+region+'/')
            os.system('cp html/index_'+region+'_'+category+'_'+era+'.html ' + output_location + '/' +category+'_'+era+'/'+region+'/index.html')
            os.system('cp index.php '+output_location+'/'+category+'_'+era+'/'+region+'/')



