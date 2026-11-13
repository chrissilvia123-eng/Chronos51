<#>
.SYNOPSIS
    Chronos 51 - GitHub Repository Setup Script
.DESCRIPTION
    Configures GitHub repository with branch protection, secrets, environments, and labels.
    Run AFTER creating the GitHub repository and pushing initial commit.
.PREREQUISITES
    - GitHub CLI (gh) installed and authenticated: `gh auth login`
    - Repository already created on GitHub
    - Pushed initial commit to main branch
.NOTES
    Run from: E:\Chronos51\05_UE5_Project\Chronos51\
#>

param(
    [Parameter(Mandatory=$true)]
    [string]$RepoOwner,
    
    [Parameter(Mandatory=$true)]
    [string]$RepoName,
    
    [switch]$SkipSecrets,
    [switch]$SkipBranchProtection,
    [switch]$SkipEnvironments,
    [switch]$SkipLabels,
    [switch]$SkipProjectBoard
)

$ErrorActionPreference = "Stop"

Write-Host "==========================================" -ForegroundColor Cyan
Write-Host "Chronos 51 - GitHub Repository Setup" -ForegroundColor Cyan
Write-Host "==========================================" -ForegroundColor Cyan
Write-Host "Repository: $RepoOwner/$RepoName"
Write-Host ""

# Verify gh CLI
if (-not (Get-Command gh -ErrorAction SilentlyContinue)) {
    Write-Error "GitHub CLI (gh) not found. Install: winget install GitHub.cli"
    exit 1
}

# Verify auth
$authStatus = gh auth status 2>&1
if ($authStatus -notmatch "Logged in") {
    Write-Error "Not authenticated with GitHub. Run: gh auth login"
    exit 1
}

Write-Host "[1/6] Configuring repository settings..." -ForegroundColor Green
gh repo edit "$RepoOwner/$RepoName" `
    --enable-issues `
    --enable-wiki=false `
    --enable-projects `
    --delete-branch-on-merge `
    --allow-squash-merge `
    --allow-merge-commit=false `
    --allow-rebase-merge=false `
    --enable-auto-merge

# Branch Protection
if (-not $SkipBranchProtection) {
    Write-Host "[2/6] Setting up branch protection for main..." -ForegroundColor Green
    
    gh api --method PUT "repos/$RepoOwner/$RepoName/branches/main/protection" `
        --field required_status_checks='{"strict":true,"contexts":["build-windows","build-linux","lint","test"]}' `
        --field enforce_admins=true `
        --field required_pull_request_reviews='{"required_approving_review_count":1,"dismiss_stale_reviews":true,"require_code_owner_reviews":true}' `
        --field restrictions='{}' `
        --field allow_force_pushes=false `
        --field allow_deletions=false `
        --field required_linear_history=true `
        --field allow_auto_merge=true
    
    Write-Host "  Branch protection configured" -ForegroundColor Gray
}

# Environments
if (-not $SkipEnvironments) {
    Write-Host "[3/6] Creating environments..." -ForegroundColor Green
    
    # Staging environment
    gh api --method PUT "repos/$RepoOwner/$RepoName/environments/staging" `
        --field wait_timer=0 `
        --field 'deployment_branch_policy={"protected_branches":true,"custom_branch_policies":false}' `
        --field 'reviewers=[{"type":"User","id":'$ReviewerId'}]' 2>$null || `
    gh api --method PUT "repos/$RepoOwner/$RepoName/environments/staging" `
        --field wait_timer=0 `
        --field 'deployment_branch_policy={"protected_branches":true,"custom_branch_policies":false}'
    
    Write-Host "  staging environment created" -ForegroundColor Gray
    
    # Production environment
    gh api --method PUT "repos/$RepoOwner/$RepoName/environments/production" `
        --field wait_timer=300 `
        --field 'deployment_branch_policy={"protected_branches":true,"custom_branch_policies":false}'
    
    Write-Host "  production environment created" -ForegroundColor Gray
}

# Labels
if (-not $SkipLabels) {
    Write-Host "[4/6] Creating labels..." -ForegroundColor Green
    
    $labels = @(
        @{name="P0 - Critical"; color="B60205"; description="Blocker - stops all work"}
        @{name="P1 - High"; color="D93F0B"; description="High priority"}
        @{name="P2 - Medium"; color="FBCA04"; description="Medium priority"}
        @{name="P3 - Low"; color="0E8A16"; description="Low priority / nice to have"}
        
        @{name="bug"; color="D73A4A"; description="Something isn't working"}
        @{name="feature"; color="A2EEEF"; description="New feature or request"}
        @{name="tech-debt"; color="F9D0C4"; description="Refactoring / cleanup needed"}
        @{name="art"; color="D4C5F9"; description="Art asset work"}
        @{name="design"; color="FEF2C0"; description="Game design work"}
        @{name="narrative"; color="C2E0C6"; description="Story / dialogue work"}
        @{name="audio"; color="FFDAB9"; description="Audio / music work"}
        @{name="pipeline"; color="E4E6EB"; description="Pipeline / tooling work"}
        @{name="documentation"; color="BFDADC"; description="Documentation updates"}
        
        @{name="code"; color="1D76DB"; description="Engineering work"}
        @{name="art"; color="D4C5F9"; description="Art asset work"}
        @{name="design"; color="FEF2C0"; description="Game design work"}
        @{name="narrative"; color="C2E0C6"; description="Story / dialogue work"}
        @{name="audio"; color="FFDAB9"; description="Audio / music work"}
        @{name="qa"; color="F9D0C4"; description="Quality assurance"}
        @{name="production"; color="E4E6EB"; description="Production / planning"}
        
        @{name="M0 - Foundation"; color="0052CC"; description="Sprint 1-2: Core systems playable"}
        @{name="M1 - Vertical Slice"; color="172B4D"; description="Sprint 3-6: Reactor Core slice"}
        @{name="M2 - Alpha"; color="091E42"; description="Sprint 7-14: All zones playable"}
        @{name="M3 - Beta"; color="1D76DB"; description="Sprint 15-24: Content complete"}
        @{name="M4 - Gold"; color="0052CC"; description="Sprint 25-28: Launch ready"}
        
        @{name="needs-triage"; color="FBCA04"; description="Needs initial review"}
        @{name="needs-design"; color="D93F0B"; description="Needs design approval"}
        @{name="blocked"; color="B60205"; description="Blocked by external dependency"}
        @{name="ready-for-qa"; color="0E8A16"; description="Ready for QA testing"}
    )
    
    foreach ($label in $labels) {
        gh label create "$($label.name)" --color "$($label.color)" --description "$($label.description)" --repo "$RepoOwner/$RepoName" 2>$null || `
        gh label edit "$($label.name)" --color "$($label.color)" --description "$($label.description)" --repo "$RepoOwner/$RepoName"
    }
    Write-Host "  Labels created/updated" -ForegroundColor Gray
}

# Secrets (requires manual input for security)
if (-not $SkipSecrets) {
    Write-Host "[5/6] Setting up repository secrets..." -ForegroundColor Green
    Write-Host "  The following secrets MUST be set manually in GitHub UI:" -ForegroundColor Yellow
    Write-Host "  Settings > Secrets and variables > Actions > New repository secret" -ForegroundColor Gray
    Write-Host "" -ForegroundColor Gray
    Write-Host "  EPIC_GAMES_TOKEN      - Epic Games API token (for UAT/BuildCookRun)" -ForegroundColor Gray
    Write-Host "  DISCORD_WEBHOOK       - Discord webhook URL for build notifications" -ForegroundColor Gray
    Write-Host "  STEAM_APP_ID          - Steam App ID (for SteamPipe upload)" -ForegroundColor Gray
    Write-Host "  STEAM_USERNAME        - Steamworks username" -ForegroundColor Gray
    Write-Host "  STEAM_PASSWORD        - Steamworks password" -ForegroundColor Gray
    Write-Host "  ITCH_API_KEY          - itch.io API key (for butler push)" -ForegroundColor Gray
    Write-Host "  ITCH_USER             - itch.io username" -ForegroundColor Gray
    Write-Host "" -ForegroundColor Gray
}

# Repository Variables
Write-Host "[6/6] Setting repository variables..." -ForegroundColor Green
gh variable set UE5_VERSION --body "5.3" --repo "$RepoOwner/$RepoName"
gh variable set PROJECT_NAME --body "Chronos51" --repo "$RepoOwner/$RepoName"
gh variable set DEFAULT_BRANCH --body "main" --repo "$RepoOwner/$RepoName"
Write-Host "  Variables set" -ForegroundColor Gray

Write-Host "" -ForegroundColor Cyan
Write-Host "==========================================" -ForegroundColor Cyan
Write-Host "GitHub Repository Setup Complete!" -ForegroundColor Cyan
Write-Host "==========================================" -ForegroundColor Cyan
Write-Host ""
Write-Host "Next steps:" -ForegroundColor Yellow
Write-Host "1. Add secrets manually in GitHub UI (see above)" -ForegroundColor Gray
Write-Host "2. Configure self-hosted runners (windows-ue5, linux-ue5)" -ForegroundColor Gray
Write-Host "3. Create GitHub Project board: 'Chronos 51 Development'" -ForegroundColor Gray
Write-Host "4. Set up milestone tracking (M0-M4)" -ForegroundColor Gray
Write-Host ""