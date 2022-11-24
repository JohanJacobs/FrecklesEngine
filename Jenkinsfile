pipeline {
	agent any
	
	stages {
		stage('downloading...') {
			steps {	
				git(url: 'https://github.com/JohanJacobs/FrecklesEngine', branch: 'main')
			}
		}
    
		stage('premake'){
			steps{
				premake5 gmake2
			}
		}
		
		stage('building'){
			steps{
				make
			}
		}
		stage('Test'){
			steps{
				echo 'No testing implemented'
			}
		}
    }
}
